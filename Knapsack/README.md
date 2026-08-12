# 0/1 Knapsack Problemi

0/1 Knapsack Problemi, her biri belirli bir **ağırlığa** ve **değere** sahip bir dizi eşya arasından, sabit kapasiteli bir çantaya, toplam değeri en yüksek olacak şekilde eşya seçimi yapmayı amaçlayan klasik bir kombinatoryal optimizasyon problemidir.

### Önemli iki kural vardır:

1. Çantanın kapasitesi asla aşılamaz (Ağırlıkların toplamı, çantanın taşıyabileceği maksimum ağırlığı geçemez).
2. Her eşya ya **bütünüyle** alınır ya da **hiç** alınmaz (Bin Packing'in aksine eşyalar bölünemez; bu nedenle problem "0/1" olarak adlandırılır).

Amaç, bu iki şartı sağlayan ve toplam değeri en üst düzeye çıkaran eşya kombinasyonunu bulmaktır.

### Problemin Zorluğu:

0/1 Knapsack'in en önemli zorluğu, Bin Packing gibi NP-Hard sınıfında bir problem olmasıdır. Eşya sayısı arttıkça "hangi eşyaları alsam en yüksek değere ulaşırım?" sorusunun olası kombinasyon sayısı `2^N` ile katlanarak artar; çünkü her eşya için yalnızca iki durum (al/alma) vardır.

Bütün kombinasyonları tek tek deneyerek (saf özyineleme / Brute Force) kesin sonucu bulmak, eşya sayısı arttıkça pratik olmaktan çıkar. Ancak Bin Packing'den farklı olarak, 0/1 Knapsack **optimal alt yapı** ve **örtüşen alt problemler** özelliklerini taşıdığından, dinamik programlama (memoization / bottom-up) teknikleriyle **kesinlikten ödün vermeden** çok daha hızlı çözülebilir. Ayrıca Branch and Bound gibi yöntemlerle, karar ağacının umut vermeyen dalları hiç gezilmeden elenerek pratikte ciddi hız kazanımları elde edilebilir.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Recursive (Saf Özyineleme) | Her eşya için "al/alma" kararının bütün kombinasyonlarını dener | Evet | `O(2^N)` | `O(N)` |
| Memoization (Top-Down DP) | Saf özyinelemeyi, hesaplanan alt problemleri bir tabloda saklayarak hızlandırır | Evet | `O(N × Kapasite)` | `O(N × Kapasite)` |
| Bottom-Up (Tabulation DP) | Alt problemleri küçükten büyüğe doğru döngülerle bir tabloya doldurur | Evet | `O(N × Kapasite)` | `O(N × Kapasite)` |
| Branch and Bound | Karar ağacını üst sınır (bound) hesabıyla budayarak umut vermeyen dalları eler | Evet | En kötü `O(2^N)`, iyi durumda çok daha hızlı | En kötü `O(2^N)` |

> `N`: eşya sayısı, `Kapasite`: çantanın taşıyabileceği maksimum ağırlık.

- **Recursive**, problemin mantığını en saf hâliyle yansıtır ve anlaşılması kolaydır, ancak örtüşen alt problemleri tekrar tekrar hesapladığı için büyük girdilerde kullanılamaz hâle gelir.
- **Memoization**, saf özyinelemenin yapısını korurken hesaplanan sonuçları bir tabloda saklayarak aynı alt problemin tekrar çözülmesini engeller; yalnızca gerçekten ihtiyaç duyulan alt problemleri hesaplar.
- **Bottom-Up**, özyinelemeyi tamamen ortadan kaldırıp tabloyu döngülerle sistemli bir sırayla doldurur; çağrı yığını riski taşımaz ve bellek kullanımı daha öngörülebilirdir.
- **Branch and Bound**, dinamik programlamadan farklı bir strateji izler: tabloya değil, karar ağacına dayanır ve umut vermeyen dalları teorik bir üst sınır hesabıyla erken eler. Pratikte, özellikle değer/ağırlık oranları birbirinden belirgin şekilde farklı eşya kümelerinde, diğer yöntemlerden çok daha az işlemle sonuca ulaşabilir.

## Algoritmaların Tercih Edilme Durumları

### Recursive (Saf Özyineleme)

**Önceki yöntemlere göre avantajı nedir?** Problemin özyinelemeli yapısını (optimal alt yapı) doğrudan koda yansıtır; iç içe döngüler veya bit maskeleri gibi karmaşık yapılara ihtiyaç duymadan, her eşya için basit bir "al/alma" kararıyla kesin sonucu bulur.

**Hangi durumda tercih edilir?** Eşya sayısının çok küçük olduğu, problemin mantığının öğretilmesinin amaçlandığı veya dinamik programlama çözümüne geçmeden önce doğru bir referans (baseline) çözüm elde edilmek istenen durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Eşya sayısının fazla olduğu durumlarda tercih edilmez. Örtüşen alt problemler tekrar tekrar hesaplandığından, çözüm süresi eşya sayısıyla birlikte katlanarak (üstel) artar.

### Memoization (Top-Down DP)

**Önceki yönteme göre avantajı nedir?** Saf özyinelemedeki gereksiz tekrar hesaplamaları, hesaplanan her alt problemi bir tabloya kaydederek ortadan kaldırır. Kodun özyinelemeli yapısı korunurken zaman karmaşıklığı `O(2^N)`'den `O(N × Kapasite)`'ye düşürülür.

**Hangi durumda tercih edilir?** Problemin özyinelemeli (top-down) yapısının korunmasının istendiği, ancak yalnızca gerçekten ihtiyaç duyulan alt problemlerin hesaplanmasının (bottom-up'ın aksine bütün tabloyu doldurmamanın) avantaj sağladığı durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Kapasitenin çok büyük olduğu ve `dp` tablosunun bellek olarak çok yer kapladığı, ya da eşya sayısının fazlalığı nedeniyle özyinelemeli çağrı yığınının (call stack) taşma riski taşıdığı durumlarda tercih edilmez.

### Bottom-Up (Tabulation DP)

**Önceki yönteme göre avantajı nedir?** Özyinelemeyi tamamen ortadan kaldırarak çağrı yığını riskini yok eder. Tablo, küçükten büyüğe doğru sistemli bir sırayla doldurulduğundan bellek kullanımı ve çalışma süresi daha öngörülebilirdir.

**Hangi durumda tercih edilir?** Eşya sayısının büyük olduğu, özyinelemeli çağrı derinliğinin sorun yaratabileceği veya performansın/bellek kullanımının öngörülebilir olmasının önemli olduğu durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Kapasitenin çok büyük olduğu durumlarda tercih edilmez; çünkü tablo, gerçekten ihtiyaç duyulup duyulmadığına bakılmaksızın baştan sona tamamen doldurulur ve bu bellek/işlem maliyetine yol açar.

### Branch and Bound

**Önceki yöntemlere göre avantajı nedir?** Dinamik programlama tablosuna bağlı kalmadan, her dala girmeden önce o daldan devam edilirse ulaşılabilecek teorik üst sınırı hesaplar; bu üst sınır mevcut en iyi çözümü geçemiyorsa dal hiç incelenmeden elenir. Bu sayede, özellikle kapasitenin çok büyük olduğu durumlarda DP tablosunun getirdiği bellek sınırlamasını aşabilir.

**Hangi durumda tercih edilir?** Kapasitenin (`W`) çok büyük olduğu, dolayısıyla `O(N × Kapasite)` boyutunda bir DP tablosunun pratik olmadığı durumlarda; ayrıca eşyaların değer/ağırlık oranlarının belirgin farklılıklar gösterdiği, dolayısıyla üst sınır hesabının dalları etkili biçimde eleyebildiği durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Bütün eşyaların değer/ağırlık oranlarının birbirine çok yakın olduğu durumlarda tercih edilmez; bu durumda üst sınır hesabı dalları etkili şekilde eleyemez ve algoritma pratikte saf özyinelemeye yakın (üstel) bir performansa döner. Kapasitenin ve eşya sayısının zaten küçük olduğu, DP çözümlerinin hızlı ve basit çalıştığı durumlarda da ek karmaşıklığı gereksizdir.

