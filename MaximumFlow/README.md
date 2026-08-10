# Maximum Flow (Maksimum Akış) Problemi

Maksimum Akış Problemi, kenarlarının belirli bir taşıma **kapasitesi** olduğu yönlü bir ağda (flow network), bir **kaynak (source)** düğümden bir **hedef (sink)** düğüme gönderilebilecek en yüksek akış miktarını bulmayı amaçlayan klasik bir graf optimizasyon problemidir.

### Önemli iki kural vardır:

1. Hiçbir kenardan, o kenarın kapasitesinden fazla akış geçirilemez (kapasite kısıtı).
2. Kaynak ve hedef dışındaki her düğümde, düğüme giren toplam akış ile düğümden çıkan toplam akış birbirine eşit olmalıdır (akış korunumu).

Amaç, bu iki şartı sağlayan tüm olası akış dağılımları arasından kaynaktan hedefe gönderilen toplam akış miktarını maksimize eden çözümü bulmaktır.

### Problemin Zorluğu:

Bin Packing gibi NP-Hard bir problemin aksine Maksimum Akış, polinomsal zamanda **kesin (exact)** olarak çözülebilen bir problemdir; bu yüzden burada asıl mesele "kesin sonucu bulabilmek" değil, bu kesin sonucu **ne kadar verimli** bulabildiğimizdir.

Naif yaklaşımlar (akışı sadece ileri yönde artırmaya çalışmak) bir kez yanlış yönde gönderilen akışı geri alamaz ve tıkanabilir. Bu problemi çözmek için geliştirilen **residual graph (kalan kapasite grafiği)** fikri, gönderilen akışın gerekirse dolaylı olarak geri alınmasına izin vererek kesin maksimum akışın bulunmasını garanti eder. Bu residual graph fikri üzerine kurulu, artırıcı yolu (augmenting path) bulma stratejisi farklı üç klasik algoritma (Ford-Fulkerson, Edmonds-Karp, Dinic) burada karşılaştırılmaktadır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yol Arama Yöntemi | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Ford-Fulkerson | Özyinelemeli DFS ile ilk bulunan yol | Evet (tam sayı kapasitede) | `O(E × max_flow)` | `O(V²)` |
| Edmonds-Karp | BFS ile en kısa (en az kenarlı) yol | Evet | `O(V³ × E)` | `O(V²)` |
| Dinic | Katmanlı BFS + bloklama akışı (DFS) | Evet | `O(V² × E)` | `O(V + E)` |

- **Ford-Fulkerson**, en basit ve en sezgisel yaklaşımdır; artırıcı yolu bulmak için özyinelemeli DFS kullanır. Ancak bulduğu yol "en kısa" değil "ilk bulduğu" yoldur, bu da çalışma süresinin kenar kapasitelerine bağlı (pseudo-polinomsal) olmasına yol açar.
- **Edmonds-Karp**, aynı residual graph mantığını kullanır fakat yol aramada DFS yerine BFS uygular. Böylece her zaman en kısa artırıcı yol bulunur ve çalışma süresi kapasitelerden bağımsız, kesin polinomsal bir sınıra kavuşur.
- **Dinic**, BFS ile katmanlı bir seviye grafı oluşturup bu grafın üzerinde DFS ile aynı anda birden fazla artırıcı yolu (bloklama akışı) bulur. Bu sayede yoğun graflarda Edmonds-Karp'tan çok daha hızlı çalışır ve komşuluk listesi kullandığı için bellek açısından da daha tasarrufludur.

## Algoritmaların Tercih Edilme Durumları

### Ford-Fulkerson (DFS Tabanlı)

**Önceki yöntemlere göre avantajı nedir?** Akışı doğrudan artırmaya çalışan naif yaklaşımların aksine, residual graph üzerindeki geri kenarlar sayesinde daha önce gönderilen akışın gerekirse geri alınmasına izin vererek kesin maksimum akışı garanti eder. Kodlanması, BFS tabanlı alternatiflere göre daha basittir; DFS özyinelemeli bir fonksiyonla doğrudan ifade edilebilir.

**Hangi durumda tercih edilir?** Kapasite değerlerinin küçük/tam sayı olduğu, ağın küçük olduğu ve hızlı, basit bir implementasyonun yeterli olduğu durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Kenar kapasitelerinin büyük olduğu graflarda, DFS çok sayıda küçük artırıcı yol bulabileceğinden algoritma gereksiz yere yavaşlar. Bu gibi durumlarda BFS tabanlı Edmonds-Karp veya Dinic tercih edilir.

### Edmonds-Karp (BFS Tabanlı)

**Önceki yöntemlere göre avantajı nedir?** Ford-Fulkerson'daki kapasiteye bağımlılık sorununu ortadan kaldırır. Her zaman en kısa artırıcı yolu bulduğu için çalışma süresi doğrudan düğüm ve kenar sayısına bağlı, kapasitelerden bağımsız polinomsal bir sınıra sahiptir.

**Hangi durumda tercih edilir?** Kenar kapasitelerinin çok büyük değerler alabildiği ve performans garantisi (polinomsal zaman) gereken; aynı zamanda implementasyonun Dinic'e kıyasla daha basit kalması istenen durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Çok büyük ve yoğun ağlarda performansın kritik olduğu durumlarda; bu gibi durumlarda Dinic gibi daha hızlı algoritmalar tercih edilir.

### Dinic

**Önceki yöntemlere göre avantajı nedir?** İki fazlı (BFS + DFS) yapısı sayesinde aynı uzunluktaki tüm artırıcı yolları tek bir geçişte bulur; Edmonds-Karp'ın her seferinde tek bir yol için tüm grafı baştan taramasının getirdiği israfı ortadan kaldırır. Ayrıca komşuluk listesi kullandığı için matris tabanlı diğer iki yönteme göre çok daha az bellek harcar.

**Hangi durumda tercih edilir?** Büyük ölçekli ve yoğun graflarda, performansın ve hızın kritik olduğu yarışma programlaması senaryolarında ve özellikle bipartite eşleştirme problemlerinde (burada `O(E√V)` gibi çarpıcı bir hıza ulaşır) tercih edilir.

**Hangi durumda tercih edilmez?** Çok küçük graflarda, BFS ve DFS'in iç içe kullanılmasının getirdiği kod karmaşıklığına değmeyebilir; bu durumlarda Edmonds-Karp yeterli ve daha anlaşılır bir çözüm sunar.