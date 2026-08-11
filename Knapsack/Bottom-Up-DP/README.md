# Bottom-Up DP Çözümü - 0/1 Knapsack Problemi

Bu bölümde, 0/1 Knapsack probleminin, özyineleme kullanmadan, sonuçları küçük alt problemlerden başlayarak döngülerle bir tabloya doldurmaya dayanan **bottom-up (tabulation) dinamik programlama** çözümü açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Memoization (top-down) çözümündeki özyinelemeli fonksiyon çağrılarının getirdiği çağrı yığını (call stack) yükünü ortadan kaldırmak; alt problemleri en küçüğünden en büyüğüne doğru sistemli bir sırayla, döngüler kullanarak doğrudan hesaplamak için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Memoization çözümünde her çağrı fonksiyon çağrı yığınına yeni bir kayıt eklediğinden, çok büyük `n` değerlerinde yığın taşması (stack overflow) riski oluşabilir. Bottom-up yaklaşım, özyineleme yerine iç içe `for` döngüleri kullanarak bu riski tamamen ortadan kaldırır.
- **Önceki yönteme (memoization) göre avantajı nedir?** Fonksiyon çağrı maliyeti olmadığı için genellikle daha hızlı çalışır ve daha öngörülebilir bellek kullanımı sağlar. Ayrıca tablo, küçükten büyüğe doğru sistemli bir sırayla dolduğundan, hangi alt problemlerin ne zaman hesaplandığı açıkça bellidir; bu da hata ayıklamayı (debugging) kolaylaştırır.
- **Hangi durumda tercih edilir?**
  - Eşya sayısının (`n`) büyük olduğu ve özyinelemeli çağrı derinliğinin sorun yaratabileceği durumlarda.
  - Performansın ve bellek kullanımının öngörülebilir olmasının önemli olduğu durumlarda.
  - Bütün `(esya, kapasite)` alt problemlerinin sonuçta zaten hesaplanması gerektiği, yani memoization'ın sağladığı "yalnızca gerekli alt problemleri hesaplama" avantajının önemli olmadığı durumlarda.
- **Hangi durumda tercih edilmez?**
  - Kapasitenin (`W`) çok büyük olduğu durumlarda; çünkü tablo, gerçekten ihtiyaç duyulup duyulmadığına bakılmaksızın `(n + 1) × (W + 1)` boyutunda baştan sona doldurulur ve bu bellek/işlem maliyetine yol açar.
  - Yalnızca birkaç alt problemin sonucuna ihtiyaç duyulan durumlarda; bu gibi durumlarda memoization, gereksiz hücrelerin hesaplanmasını önleyerek daha verimli olabilir.

## Algoritmanın Çalışma Mantığı

Fonksiyon, `(n + 1) × (W + 1)` boyutunda bir `dp` tablosu oluşturur. Tablonun satırları değerlendirilen eşya sayısını (`i`), sütunları ise çanta kapasitesini (`j`) temsil eder. Tablo, en küçük alt problemlerden (`i = 0` veya `j = 0`) başlanarak, iç içe iki döngü ile satır satır doldurulur:

1. **Taban durum:** Hiç eşya değerlendirilmediyse (`i == 0`) veya kapasite `0` ise (`j == 0`), o hücreye elde edilebilecek maksimum değer olarak `0` yazılır.
2. **Eşyayı alma (pick):** `i`. eşyanın ağırlığı (`wt[i-1]`), mevcut kapasiteye (`j`) sığıyorsa, bu eşyanın değeri ile `dp[i-1][j - wt[i-1]]` hücresindeki (bir önceki eşya grubunda, kalan kapasiteyle ulaşılan) değer toplanır.
3. **Eşyayı almama (notPick):** `dp[i-1][j]` hücresindeki değer, yani bir önceki eşya grubuyla aynı kapasitede ulaşılan değer alınır.
4. **Hücreyi doldurma:** `dp[i][j]`, `pick` ve `notPick` değerlerinin büyüğü olarak atanır.
5. Bütün tablo bu şekilde doldurulduktan sonra, `dp[n][W]` hücresi, bütün eşyalar değerlendirildiğinde `W` kapasiteyle ulaşılabilecek maksimum değeri verir.

Bu yaklaşımda özyineleme yoktur; her hücre yalnızca kendisinden önce hesaplanmış olan (bir üst satırdaki) hücrelere bakarak doğrudan hesaplanır.

### Örnek

Aşağıdaki girdiler verilsin:

```text
val = {1, 2, 3}
wt  = {4, 5, 1}
W = 4
```

Tablo, `i = 0` satırının tamamı `0` olarak doldurularak başlar (hiç eşya yokken hangi kapasitede olursa olsun değer `0`'dır). Ardından `i = 1` satırında (yalnızca ağırlığı `4`, değeri `1` olan ilk eşya değerlendirilirken) `j = 4` sütununa gelindiğinde eşya kapasiteye tam sığdığı için `dp[1][4] = 1` olur. Tablo bu şekilde satır satır doldurulmaya devam eder; üçüncü eşya (ağırlık `1`, değer `3`) değerlendirilirken düşük ağırlığı sayesinde birçok kapasite değerine kolayca sığar ve toplam değeri artırır. Bütün satırlar dolduktan sonra `dp[3][4]` hücresi, `W = 4` kapasitesiyle ulaşılabilecek maksimum değeri verir.

> Bottom-up yaklaşımda tablo her zaman **baştan sona tamamen** doldurulur; bu, memoization'ın aksine bazı alt problemlerin hiç hesaplanmadan atlanamayacağı anlamına gelir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon Knapsack(W, Degerler, Agirliklar):

    n = Agirliklar boyutu

    // dp tablosunu oluştur: (n+1) satır, (W+1) sütun
    dp = (n+1) x (W+1) boyutunda tablo

    Döngü (i = 0'dan n'e kadar):
        Döngü (j = 0'dan W'ya kadar):

            Eğer i == 0 VEYA j == 0 ise:
                dp[i][j] = 0

            Değilse:
                Pick = 0
                Eğer Agirliklar[i - 1] <= j ise:
                    Pick = Degerler[i - 1] + dp[i - 1][j - Agirliklar[i - 1]]

                NotPick = dp[i - 1][j]

                dp[i][j] = Maksimum(Pick, NotPick)

    Döndür dp[n][W]
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<int> &val` ve `vector<int> &wt`** — Eşyaların değerlerini ve ağırlıklarını tutan vektörler; fonksiyona referans (`&`) ile geçirilerek gereksiz kopyalamanın önüne geçilir.

* **`vector<vector<int>> dp`** — `(n + 1) × (W + 1)` boyutunda iki boyutlu bir vektördür. `dp[i][j]`, ilk `i` eşya arasından seçim yapılarak `j` kapasitesiyle elde edilebilecek maksimum değeri saklar.

* **İç içe `for` döngüleri** — Tablonun satır satır (`i`) ve her satırda sütun sütun (`j`) doldurulmasını sağlar. Özyineleme yerine yinelemeli (iterative) bir yaklaşım kullanıldığı için fonksiyon çağrı yığını oluşmaz.

* **`if (i == 0 || j == 0)`** — Tablonun ilk satırının ve ilk sütununun taban durum değerleriyle (`0`) doldurulmasını sağlar; bu değerler, sonraki hücrelerin hesaplanmasında referans olarak kullanılır.

* **`max(pick, notPick)`** — Bir eşyanın alınması ve alınmaması durumlarından elde edilen değerlerden büyük olanını seçerek `dp[i][j]` hücresine yazılacak nihai değeri belirler.

---

## Kullanılan Kütüphaneler

```cpp
#include <vector>
#include <iostream>
```

- `<vector>`: Girdi olarak verilen `val`/`wt` vektörlerinin ve iki boyutlu `dp` tablosunun oluşturulması için kullanılır.
- `<iostream>`: `cout` kullanılarak sonucun ekrana yazdırılması için kullanılır.

> **Not:** Kodda kullanılan `max()` fonksiyonu standart olarak `<algorithm>` başlık dosyasında tanımlıdır. `<vector>` başlığının bunu dolaylı olarak içermesi bazı derleyicilerde hatasız derlemeyi sağlayabilir; ancak taşınabilirlik açısından `<algorithm>` başlığının da eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Tablo, iç içe iki döngü ile doldurulur:

```cpp
for (int i = 0; i <= n; i++)
    for (int j = 0; j <= W; j++)
```

Dış döngü `n + 1`, iç döngü `W + 1` kez çalışır ve her hücre sabit zamanda (`O(1)`) hesaplanır. Bu nedenle toplam zaman karmaşıklığı:

```text
O(N × W)
```

olur.

### Best / Average / Worst Case — O(N × W)

Tablo, girdideki ağırlık ve değerlerden bağımsız olarak her zaman baştan sona tamamen doldurulduğundan, algoritmanın en iyi, ortalama ve en kötü durum karmaşıklığı arasında fark yoktur. Her koşulda karmaşıklık:

```text
O(N × W)
```

ile sabittir. Bu davranış, girdiye göre değişkenlik gösterebilen memoization çözümünden farklı olarak, bottom-up yaklaşımın en belirgin özelliğidir.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N × W)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

`dp` tablosu, `(n + 1) × (W + 1)` boyutunda bellek kullanır:

```cpp
vector<vector<int>> dp(n + 1, vector<int>(W + 1));
```

Bu nedenle tablonun kullandığı bellek:

```text
O(N × W)
```

olur.

Bottom-up yaklaşımda özyineleme kullanılmadığı için memoization çözümünden farklı olarak fonksiyon çağrı yığınına bağlı ek bir `O(N)` bellek kullanımı yoktur. Bu, bottom-up yaklaşımın bellek kullanımını daha öngörülebilir kılan bir özelliğidir.

`val` ve `wt` vektörleri girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

> **Not:** Bu çözümde her `dp[i][j]` hücresi yalnızca bir önceki satıra (`dp[i-1][...]`) bakılarak hesaplandığından, tablo tek boyutlu bir vektöre indirgenerek alan karmaşıklığı `O(W)`'ya düşürülebilir. Ancak bu kodda, tüm ara sonuçların izlenebilir olması için iki boyutlu tablo tercih edilmiştir.