# Convex Hull (Dış Bükey Zarf) Problemi

Convex Hull Problemi, düzlemde verilen bir nokta kümesinin tamamını içine alan, en az sayıda köşeden oluşan **en küçük dış bükey (convex) çokgeni** bulmayı amaçlayan klasik bir hesaplamalı geometri problemidir.

### Önemli iki kural vardır:

1. Kümedeki hiçbir nokta, bulunan çokgenin dışında kalamaz (Bütün noktalar çokgenin içinde veya sınırında olmalıdır).
2. Çokgenin köşeleri yalnızca nokta kümesinin kendi elemanları arasından seçilebilir ve çokgen dış bükey (convex) olmak zorundadır.

Amaç, bu iki şartı sağlayan ve nokta kümesini en dar şekilde çevreleyen zarfı bulmaktır.

### Problemin Zorluğu:

Convex Hull'un en önemli zorluğu, kaba kuvvet (Brute Force) yaklaşımının nokta sayısı arttıkça pratik olmaktan çıkmasıdır. Her nokta üçlüsünün diğer bütün noktalara göre konumunu kontrol ederek zarfı bulmaya çalışmak `O(N³)` gibi yüksek bir maliyet doğurur.

Bu nedenle, bütün üçlü kombinasyonları tek tek denemek yerine, noktaları belirli bir mantıkla (açısal sıralama, en dış noktayı arama vb.) işleyerek zarfı çok daha kısa sürede ve **kesin** olarak bulan algoritmalar (Jarvis March, Graham Scan gibi) geliştirilmiştir. Bin Packing'deki Greedy yaklaşımlardan farklı olarak, bu algoritmalar kesin (optimum) sonucu garanti eder; asıl kazanım, kesinlikten ödün vermeden hızdır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Jarvis March (Gift Wrapping) | Başlangıç noktasından itibaren her adımda en dışta kalan noktayı arayarak zarfı sarar | Evet | `O(N × H)` | `O(N)` |
| Graham Scan | Noktaları bir referans noktaya göre açısal olarak sıralar, ardından yığınla zarfı oluşturur | Evet | `O(N log N)` | `O(N)` |

> `N`: toplam nokta sayısı, `H`: dış bükey zarf üzerindeki nokta sayısı.

- **Jarvis March**, mantığı en basit ve anlaşılması en kolay yöntemdir; ancak zarf üzerindeki her nokta için bütün nokta kümesini yeniden taradığından, zarf üzerindeki nokta sayısı (`H`) arttıkça yavaşlar.
- **Graham Scan**, noktaları önce bir kez sıralar ve ardından her noktayı yalnızca sabit sayıda işleme tabi tutar. Bu sayede performansı `H`'den bağımsız olarak her koşulda `O(N log N)` ile sınırlıdır ve büyük veri kümelerinde daha öngörülebilir çalışır.

## Algoritmaların Tercih Edilme Durumları

### Jarvis March (Gift Wrapping)

**Önceki yöntemlere (Kaba Kuvvete) göre avantajı nedir?** Bütün nokta üçlülerini tek tek denemek yerine, zarfın kesin olarak bir parçası olan bir başlangıç noktasından başlayıp her adımda yalnızca bir sonraki dış noktayı arayarak çok daha az işlemle sonuca ulaşır. Çalışma mantığı ve kodlanması oldukça basittir.

**Hangi durumda tercih edilir?** Dış bükey zarf üzerindeki nokta sayısının (`H`) toplam nokta sayısına (`N`) göre az olduğu durumlarda, ayrıca basit ve kolay anlaşılır bir uygulamaya ihtiyaç duyulan senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Dış bükey zarf üzerindeki nokta sayısının toplam nokta sayısına yakın olduğu durumlarda (örneğin noktaların çoğu bir çember üzerinde dizilmişse) tercih edilmez. Bu durumda karmaşıklık `O(N²)`'ye yaklaşarak performans kötüleşir.

### Graham Scan

**Önceki yöntemlere göre avantajı nedir?** Jarvis March'ın aksine noktaları tekrar tekrar taramaz. Noktaları bir kez polar açıya göre sıraladıktan sonra her noktayı yığına yalnızca bir kez ekleyip en fazla bir kez çıkararak zarfı oluşturur. Bu sayede performansı, zarf üzerindeki nokta sayısından bağımsız olarak her koşulda öngörülebilir kalır.

**Hangi durumda tercih edilir?** Nokta sayısının fazla olduğu ve dış bükey zarf üzerindeki nokta sayısının önceden kestirilemediği veya yüksek olabileceği durumlarda; garanti edilmiş `O(N log N)` performansına ihtiyaç duyulan senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Noktaların sisteme tek tek, sırayla geldiği ve zarfın anlık olarak güncellenmesi gereken çevrim içi (online) sistemlerde tercih edilmez; çünkü algoritma çalışmaya başlamadan önce bütün noktaların elde bulunması ve sıralanması gerekir.