# Graph Coloring Problemi

Graf Boyama Problemi, bir grafın düğümlerine, **birbirine komşu (kenarla bağlı) hiçbir iki düğüm aynı renge sahip olmayacak şekilde** renk atanmasını amaçlayan klasik bir kombinatoryal optimizasyon problemidir.

### Önemli iki kural vardır:

1. Birbirine kenarla bağlı (komşu) iki düğüm asla aynı renge sahip olamaz.
2. Bütün düğümler mutlaka bir renge atanmalıdır.

Amaç, bu şartları sağlayan boyamalar arasından **mümkün olan en az sayıda rengi** kullanan çözümü bulmaktır. Bir graf için kullanılması gereken minimum renk sayısına o grafın **kromatik sayısı (chromatic number)** denir.

### Problemin Zorluğu:

Graf Boyama'nın en önemli zorluğu, kromatik sayının kesin olarak bulunmasının **NP-Zor (NP-Hard)** bir problem olmasıdır. Düğüm sayısı arttıkça "hangi düğüme hangi rengi versem toplam renk sayısı en az olur?" sorusunun olası boyama kombinasyonları astronomik seviyelere ulaşır.

Tüm kombinasyonları tek tek deneyerek (Brute Force) veya geri izleme (Backtracking) ile kesin ve mükemmel sonucu aramak, düğüm sayısı büyüdükçe pratikte imkansız hale gelir. Bu nedenle, Graf Boyama probleminde mükemmeli aramak yerine "mükemmele yakın" ve kabul edilebilir sonucu çok hızlı bir şekilde üreten Açgözlü (Greedy) yaklaşımlar (Heuristic algoritmalar) kullanılır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Greedy | Düğümleri sabit sırayla (`0`'dan `V-1`'e) ele alır, kullanılabilir en küçük rengi atar | Hayır | `O(V²)` | `O(V)` |
| DSATUR | Düğümleri her adımda doygunluk derecesine göre dinamik olarak seçer, kullanılabilir en küçük rengi atar | Hayır | `O(V³)` | `O(V)` |

- **Greedy**, düğümleri sabit ve önceden belirli bir sırayla ele alan, çok basit ve hızlı bir yöntemdir. Ancak sonucun kalitesi tamamen düğümlerin ele alınış sırasına bağlıdır; kötü bir sıralama gereğinden fazla renk kullanılmasına yol açabilir.
- **DSATUR**, bir sonraki boyanacak düğümü sabit sırayla değil, o an en çok "kısıtlanmış" (komşularında en fazla farklı renk bulunan) düğümü seçerek belirler. Bu sayede sıraya bağımlılığı azaltır ve pratikte Greedy'ye göre genellikle daha az veya en fazla eşit sayıda renk kullanır; ancak her adımda doygunluk derecesini yeniden hesapladığı için ekstra zaman maliyeti getirir.

## Algoritmaların Tercih Edilme Durumları

### Greedy

**Önceki yöntemlere (Kesin Çözümlere) göre avantajı nedir?** Bütün boyama kombinasyonlarını denemek yerine, her düğümü komşularının kullanmadığı en küçük renge atayarak çok daha kısa sürede, hızlı ve pratik bir çözüm üretir. Çalışma mantığı ve kodlanması oldukça basittir, karmaşık veri yapılarına ihtiyaç duymaz.

**Hangi durumda tercih edilir?**
- Düğüm sayısının fazla olduğu büyük graflarda.
- Hızlı ve kabul edilebilir bir çözüme ihtiyaç duyulduğu durumlarda.
- Kromatik sayının kesin olarak bulunmasının zorunlu olmadığı durumlarda.
- Zamanlama, kayıt tahsisi (register allocation), frekans atama gibi yaklaşık çözümün yeterli olduğu uygulamalarda.

**Hangi durumda tercih edilmez?** Minimum renk sayısının kesin olarak bulunması gereken durumlarda tercih edilmez. Düğümlerin ele alınış sırasından doğrudan etkilendiği için bazı graflarda gereğinden fazla renk kullanır ve optimum çözümden (kromatik sayıdan) uzaklaşabilir.

### DSATUR

**Önceki yöntemlere göre avantajı nedir?** Greedy algoritmasına kıyasla düğümleri sabit bir sırayla değil, o anki boyama durumuna göre dinamik olarak seçer. Komşularında daha fazla farklı renk bulunan (dolayısıyla daha "kısıtlı" olan) düğümleri önce boyayarak, ilerleyen adımlarda ortaya çıkabilecek renk çakışmalarını önceden azaltır ve genellikle daha az renk kullanan sonuçlar üretir.

**Hangi durumda tercih edilir?**
- Greedy'ye göre daha kaliteli (daha az renkli) bir çözüme ihtiyaç duyulan, ancak yine de kesin çözüm (backtracking, brute force) yöntemlerinin çok yavaş kalacağı büyük graflarda.
- Kayıt tahsisi (register allocation), zaman çizelgeleme (scheduling), harita/frekans atama gibi pratik renk sayısının önemli olduğu uygulamalarda.
- Düğüm sırasının sonucu olumsuz etkileyebileceği düzensiz yapılı graflarda.

**Hangi durumda tercih edilmez?** Minimum renk sayısının (kromatik sayının) kesin olarak garanti edilmesi gerektiği durumlarda tercih edilmez; DSATUR da Greedy gibi bir sezgisel (heuristic) yöntemdir ve optimum sonucu garanti etmez. Ayrıca çok küçük graflarda, ek doygunluk derecesi hesaplamalarının getirdiği ekstra zaman maliyeti (her adımda `O(V²)`) Greedy'nin basitliğine değmeyebilir.

