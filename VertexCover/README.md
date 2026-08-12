# Vertex Cover (Köşe Örtüsü) Problemi

Vertex Cover Problemi, verilen bir grafın **bütün kenarlarını örtecek** (yani her kenarın en az bir ucunun dahil olacağı) şekilde, mümkün olduğunca az sayıda köşe seçmeyi amaçlayan klasik bir kombinatoryal optimizasyon problemidir.

### Önemli iki kural vardır:

1. Grafın hiçbir kenarı örtüsüz kalamaz (Her kenarın en az bir ucu seçilen köşeler kümesinde bulunmalıdır).
2. Amaç, bu şartı sağlayan köşe kümeleri arasından **en az** sayıda köşe içeren kümeyi (minimum vertex cover'ı) bulmaktır.

### Problemin Zorluğu:

Vertex Cover'ın en önemli zorluğu, NP-Hard sınıfında bir problem olmasıdır. Köşe sayısı arttıkça "hangi köşeleri seçsem bütün kenarları en az köşeyle örterim?" sorusunun olası kombinasyon sayısı `2^V` ile katlanarak artar; çünkü her köşe için yalnızca iki durum (seç/seçme) vardır.

Bütün köşe kombinasyonlarını tek tek deneyerek kesin (minimum) sonucu aramak, büyük graflarda pratik olarak imkansızdır. Bu nedenle, Bin Packing ve Knapsack problemlerinde olduğu gibi, kesinlikten ödün veren **açgözlü (Greedy) yaklaşıklık algoritmaları** ile kesin sonucu garanti eden ancak yalnızca küçük graflarda uygulanabilir **Backtracking** yaklaşımları bir arada kullanılır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Greedy (2-Approximation) | Örtülmemiş her kenarın iki ucunu da örtüye ekler | Hayır (En fazla 2 kat) | `O(V + E)` | `O(V + E)` |
| Backtracking (Exact) | Her köşe için "seç/seçme" kararını budamalı özyinelemeyle dener | Evet | En kötü `O(2^V × E)` | `O(V + E)` |

- **Greedy (2-Yaklaşıklı)**, grafın kenarlarını tek geçişte dolaşarak çok hızlı bir çözüm üretir; ancak bir kenarın iki ucunu da örtüye eklediği için bulunan örtü, gerçek minimumdan gereğinden büyük olabilir. Yine de bulunan sonucun **gerçek minimumun en fazla iki katı** olacağı matematiksel olarak garantidir.
- **Backtracking (Kesin)**, bütün köşe kombinasyonlarını (gereksiz olanları budayarak) sistemli şekilde dener ve her zaman gerçek minimum vertex cover'ı bulur. Ancak problem NP-Hard olduğundan, budama olsa bile büyük graflarda üstel zaman alabilir.

## Algoritmaların Tercih Edilme Durumları

### Greedy (2-Approximation)

**Önceki yöntemlere (Kesin Çözümlere) göre avantajı nedir?** Bütün köşe kombinasyonlarını denemek yerine, grafın kenarlarını tek geçişte dolaşarak çok kısa sürede bir çözüm üretir. Çalışma mantığı ve kodlanması oldukça basittir; yalnızca komşuluk listesi üzerinde tek bir döngü gerektirir.

**Hangi durumda tercih edilir?** Köşe ve kenar sayısının fazla olduğu, kesin çözümün pratik olarak hesaplanamayacağı durumlarda; hızlı bir sonuca ihtiyaç duyulan ve "gerçek minimumun en fazla iki katı" garantisinin yeterli kabul edildiği senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Minimum vertex cover'ın kesin olarak bulunması gereken durumlarda tercih edilmez. Bir kenarın iki ucunu birden örtüye eklediği için, gereğinden fazla köşe seçilebilir ve algoritma optimum çözümden uzaklaşabilir.

### Backtracking (Exact)

**Önceki yönteme (Greedy'ye) göre avantajı nedir?** Sonuç olarak bulunan örtü her zaman **gerçek minimum** boyuttadır; yaklaşık değil, kesin bir çözümdür. Budama mekanizması sayesinde, o ana kadar bulunan en iyi çözümden zaten daha kötü olacağı kesinleşen dallar hiç incelenmeden elenir.

**Hangi durumda tercih edilir?** Köşe sayısının küçük olduğu ve minimum vertex cover'ın kesin olarak bulunmasının zorunlu olduğu durumlarda (örneğin küçük ölçekli ağ güvenliği, sensör yerleşimi gibi kesinliğin kritik olduğu senaryolarda) tercih edilir.

**Hangi durumda tercih edilmez?** Köşe sayısının fazla olduğu durumlarda tercih edilmez. Problem NP-Hard olduğundan, budama olsa dahi en kötü durumda karar ağacının büyük bir kısmı gezilebilir; bu da büyük graflarda algoritmayı pratik olmaktan çıkarır.

