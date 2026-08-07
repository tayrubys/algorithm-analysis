# MRV (Minimum Remaining Values) ile Backtracking — Sudoku Problemi

Bu bölümde Sudoku bulmacasının, her adımda sırayla bir sonraki boş hücreye geçmek yerine, **en az seçeneğe (aday sayıya) sahip boş hücreyi** önce doldurarak çözülmesini sağlayan, klasik backtracking'in **MRV (Minimum Remaining Values)** sezgiseliyle optimize edilmiş hâli açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Klasik backtracking'de hücrelerin sabit bir sırayla (soldan sağa, yukarıdan aşağıya) dolaşılması, bazı durumlarda çok sayıda geçerli adayı olan "kolay" hücrelerin erken doldurulup, az adayı olan "zor" hücrelerin sona bırakılmasına neden olur. Bu da geri alma (backtrack) sayısını artırır. MRV, her adımda en az seçeneği olan hücreyi önce çözerek, hatalı dalların çok daha erken tespit edilmesini sağlamak için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Klasik backtracking'in sabit hücre sırasının, geçersiz bir yerleşimi ancak arama ağacının derinliklerinde fark etmesi sorununu giderir. Bir hücrede hiç geçerli aday kalmadıysa (`minChoices == 0`), MRV bu durumu **çok daha erken** tespit ederek gereksiz derin aramaları önler.
- **Önceki yöntemlere göre avantajı nedir?** "En kısıtlı değişkeni önce seç" (**fail-first**) prensibiyle çalışır: bir hücrenin yalnızca `1` adayı varsa (`minChoices == 1`), o hücre hakkında neredeyse hiç belirsizlik yoktur ve doğrudan seçilerek arama derinlemesine dallanmadan ilerletilir. Bu, klasik sabit sıralı backtracking'e göre pratikte çok daha az düğüm gezilmesini sağlar.
- **Hangi durumda tercih edilir?**
  - Bulmacanın zor olduğu, çok sayıda boş hücre ve az ipucu içerdiği durumlarda; bu gibi durumlarda MRV'nin erken budama etkisi en çok fark yaratır.
  - Kesin ve geçerli bir çözümün garanti olarak bulunması gerektiği, ancak klasik backtracking'in pratikte çok yavaş kaldığı durumlarda.
- **Hangi durumda tercih edilmez?**
  - Bulmacanın zaten çok kolay olduğu (çoğu hücrenin dolu olduğu) durumlarda; bu gibi durumlarda her adımda tüm boş hücreleri tarayıp aday sayılarını hesaplamanın (`findMRVCell`'in kendi maliyeti) getirdiği ek yük, klasik backtracking'e kıyasla belirgin bir avantaj sağlamayabilir.
  - Basitlik ve kodun anlaşılırlığının öncelikli olduğu, eğitim amaçlı senaryolarda; klasik sabit sıralı backtracking daha kolay takip edilebilir.

## Algoritmanın Çalışma Mantığı

Her adımda, önce hangi hücrenin doldurulacağına karar verilir; bu seçim sabit bir sırayla değil, **en az aday sayısına sahip hücre** (`Minimum Remaining Values`) bulunarak yapılır.

Her adım için:

1. Tahtadaki tüm hücreler taranır; her boş hücre için `1`'den `9`'a kadar kaç sayının o hücreye geçerli şekilde yerleştirilebileceği (`isSafe` ile) sayılır.
2. En az geçerli adaya sahip olan boş hücre (`bestRow`, `bestCol`) seçilir.
3. Eğer bir hücrenin yalnızca `1` adayı bulunursa **veya hiç adayı** (`0`) bulunmazsa, arama hemen durdurulur ve bu hücre doğrudan seçilir; çünkü her iki durumda da daha iyisi (daha kısıtlı bir hücre) olamaz — `0` adaylı bir hücre zaten mevcut yerleşimin bir çıkmaz sokak olduğunu gösterir ve bunun en erken şekilde fark edilmesi istenir.
4. Hiç boş hücre kalmadıysa (`findMRVCell` `false` döndürürse), bulmaca tamamen ve geçerli şekilde çözülmüş demektir.
5. Seçilen hücre için `1`'den `9`'a kadar sayılar sırayla denenir; her sayı için `isSafe` ile geçerliliği kontrol edilir.
6. Sayı geçerliyse hücreye yerleştirilir ve algoritma **özyinelemeli (recursive)** olarak bir sonraki MRV hücresi için çağrılır.
7. Sonraki adım için bir çözüm bulunamazsa, az önce yerleştirilen sayı hücreden kaldırılır (**geri alma / backtrack**) ve bir sonraki sayı denenir.
8. Seçilen hücrede `1`'den `9`'a kadar hiçbir sayı geçerli olmuyorsa (`minChoices == 0` durumu dahil), fonksiyon `false` döndürerek bir önceki adıma geri döner.

### Örnek

Aşağıdaki `9 × 9` Sudoku bulmacası verilmiş olsun (`0` boş hücreleri temsil eder):

```text
3 0 6 5 0 8 4 0 0
5 2 0 0 0 0 0 0 0
0 8 7 0 0 0 0 3 1
0 0 3 0 1 0 0 8 0
9 0 0 8 6 3 0 0 5
0 5 0 0 9 0 6 0 0
1 3 0 0 0 0 2 5 0
0 0 0 0 0 0 0 7 4
0 0 5 2 0 6 3 0 0
```

Klasik backtracking'in aksine algoritma `(0,0)`'dan başlamaz; önce tüm boş hücreler taranır:

```text
findMRVCell taraması:
(0,1) hücresi için aday sayısı hesaplanır → örneğin 3 aday
(0,4) hücresi için aday sayısı hesaplanır → örneğin 4 aday
...
(1,2) hücresi için aday sayısı hesaplanır → örneğin 1 aday bulunursa
      → tarama hemen durur, bu hücre seçilir (minChoices == 1)

Seçilen hücreye tek geçerli aday yerleştirilir → devam edilir.
Bir sonraki adımda yine tüm boş hücreler yeniden taranır ve
en az adaya sahip olan hücre seçilir.
```

Algoritma bu şekilde ilerleyerek, klasik yöntemle aynı geçerli çözüme ulaşır (yalnızca hücrelerin doldurulma **sırası** farklıdır):

```text
3 1 6 5 7 8 4 9 2
5 2 9 1 3 4 7 6 8
4 8 7 6 2 9 5 3 1
2 6 3 4 1 5 9 8 7
9 7 4 8 6 3 1 2 5
8 5 1 7 9 2 6 4 3
1 3 8 9 4 7 2 5 6
6 9 2 3 5 1 8 7 4
7 4 5 2 8 6 3 1 9
```

> MRV, "önce en zor kararı ver" (**fail-first**) prensibiyle çalışır. Bir hücrede hiç aday kalmadığını erken fark etmek, o dalın tamamının gereksiz yere derinlemesine gezilmesini engeller.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon isSafe(mat, row, col, num):

    // Satır kontrolü
    Döngü (x = 0'dan 8'e kadar):
        Eğer mat[row][x] == num ise:
            Yanlış döndür

    // Sütun kontrolü
    Döngü (x = 0'dan 8'e kadar):
        Eğer mat[x][col] == num ise:
            Yanlış döndür

    // 3x3 kutu kontrolü
    startRow = row - (row mod 3)
    startCol = col - (col mod 3)

    Döngü (i = 0'dan 2'ye kadar):
        Döngü (j = 0'dan 2'ye kadar):
            Eğer mat[startRow + i][startCol + j] == num ise:
                Yanlış döndür

    Doğru döndür


Fonksiyon findMRVCell(mat, bestRow, bestCol):   // bestRow, bestCol referans olarak güncellenir

    minChoices = 10   // en fazla 9 seçenek olabilir
    found = Yanlış

    // Tahtadaki tüm hücreleri tara
    Döngü (row = 0'dan 8'e kadar):
        Döngü (col = 0'dan 8'e kadar):

            Eğer mat[row][col] == 0 ise:

                choices = 0

                // Bu hücreye kaç sayı yerleşebilir?
                Döngü (num = 1'den 9'a kadar):
                    Eğer isSafe(mat, row, col, num) ise:
                        choices = choices + 1

                // Daha az seçeneği olan hücre bulunduysa güncelle
                Eğer choices < minChoices ise:
                    minChoices = choices
                    bestRow = row
                    bestCol = col
                    found = Doğru

                    // Daha iyisi olamaz (0 = çıkmaz sokak, 1 = tek zorunlu aday), taramayı hemen durdur
                    Eğer minChoices == 0 VEYA minChoices == 1 ise:
                        Doğru döndür

    Döndür found


Fonksiyon solveSudoku(mat):

    // Boş hücre kalmadıysa çözüm bulunmuştur
    Eğer findMRVCell(mat, row, col) == Yanlış ise:
        Doğru döndür

    // Seçilen hücre için sayıları dene
    Döngü (num = 1'den 9'a kadar):
        Eğer isSafe(mat, row, col, num) ise:

            mat[row][col] = num

            Eğer solveSudoku(mat) == Doğru ise:
                Doğru döndür

            // Backtracking
            mat[row][col] = 0

    Yanlış döndür
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<vector<int>> &mat`** — Sudoku tahtasını iki boyutlu bir vektör olarak referans ile tutar. Önceki `isSafe`-tabanlı sürümle aynıdır.

* **`isSafe(mat, row, col, num)`** — Belirli bir `(row, col)` hücresine `num` sayısını yerleştirmenin geçerli olup olmadığını satır, sütun ve `3×3` kutu kontrolüyle sınar. Bu sürümde ayrıca `findMRVCell` içinde, her hücrenin **kaç geçerli adayı olduğunu saymak** için tekrar tekrar çağrılır.

* **`findMRVCell(mat, bestRow, bestCol)`** — Algoritmanın kalbidir. Tüm boş hücreleri tarayarak her biri için geçerli aday sayısını (`choices`) hesaplar ve en az adaya sahip hücreyi `bestRow`, `bestCol` referans parametrelerine yazar.

* **`int minChoices`** — O ana kadar bulunan en küçük aday sayısını tutar; başlangıç değeri `10` verilir (9'dan büyük), böylece taranan ilk boş hücre otomatik olarak aday olur.

* **`bool found`** — Tahtada en az bir boş hücre bulunup bulunmadığını takip eden bayrak (flag) değişkendir; tüm hücreler doluysa `false` kalır ve bu, çözümün tamamlandığı anlamına gelir.

* **Erken çıkış (`if (minChoices == 0 || minChoices == 1) return true;`)** — `findMRVCell` içindeki önemli bir optimizasyondur ve iki farklı durumu kapsar:
  * `minChoices == 1`: hücrenin yalnızca tek bir geçerli adayı vardır; bundan daha iyisi (daha kısıtlı bir hücre) olamayacağı için tarama hemen durdurulur.
  * `minChoices == 0`: hücrenin **hiç** geçerli adayı yoktur; bu, mevcut kısmi yerleşimin bir çıkmaz sokağa (dead end) girdiğini gösterir. Bu durumun da erken yakalanması, `solveSudoku`'nun bu hücreye hiç sıra gelmeden, dolayısıyla arama ağacının çok daha derinlerine inmeden geri dönmesini sağlar (**fail-fast**).

* **`int &bestRow, int &bestCol` (referans parametreler)** — `findMRVCell` fonksiyonunun, tek bir değer döndürmek yerine iki değeri (satır ve sütun) aynı anda çağıran fonksiyona geri iletmesini sağlar.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

* `<iostream>`: `cout` kullanılarak çözülen tahtanın ekrana yazdırılması için kullanılır.
* `<vector>`: Sudoku tahtasını iki boyutlu, dinamik boyutlu bir yapı (`vector<vector<int>>`) olarak saklamak için kullanılır.

---

## Zaman Karmaşıklığı

Algoritmanın taban durumu (base case), `findMRVCell` fonksiyonunun tahtada artık boş hücre bulamadığı ana karşılık gelir:

```cpp
if (!findMRVCell(mat, row, col))
    return true;
```

`findMRVCell()` `false` döndürüyorsa tahtada **hiç boş hücre kalmamıştır**. Bu durumda Sudoku çözülmüştür ve `true` döndürülür.

`minChoices == 0` ise taban durum değildir; bu durumda seçilen hücreye hiçbir sayı yerleştirilemez, `solveSudoku()` `false` döndürür ve **backtracking** yapılır.

### Average Case — Değişken, Klasik Yönteme Göre Genellikle Daha Az Düğüm

Ortalama durumda, MRV sezgiselinin fail-first yaklaşımı sayesinde gezilen düğüm sayısı klasik backtracking'e kıyasla belirgin şekilde azalır. Ancak her düğümde `findMRVCell`'in kendisi daha pahalı olduğu için, toplam performans; bulmacanın zorluğuna, ipucu dağılımına ve budamanın ne kadar erken gerçekleştiğine bağlıdır.

### Worst Case — O(9^(N × N))

Teorik en kötü durum üst sınırı, klasik backtracking ile aynıdır; çünkü MRV yalnızca hücre seçim **sırasını** değiştirir, arama uzayının kendisini garantili olarak küçültmez. Bu nedenle en kötü durum zaman karmaşıklığı hâlâ:

```text
O(9^(N × N))
```

mertebesindedir. Ancak pratikte, özellikle çoğu Sudoku bulmacasında, MRV'nin erken budama etkisi gezilen düğüm sayısını klasik yönteme göre çarpıcı biçimde azaltır.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N²)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

`mat`, fonksiyona dışarıdan referans ile aktarıldığı için kendisi ek bellek kullanımına dahil edilmez; yalnızca üzerinde yerinde (in-place) değişiklik yapılır.

`solveSudoku` fonksiyonu özyinelemeli olarak çağrıldığı için, çağrı yığını en fazla boş hücre sayısı kadar (`en fazla N × N`) derinliğe ulaşabilir. Bu nedenle özyineleme yığını `O(N²)` ek alan tüketir.

`findMRVCell` fonksiyonu özyinelemeli değildir ve yalnızca `minChoices`, `found`, `row`, `col`, `choices` gibi sabit sayıda değişken kullanır; bu nedenle her çağrıda `O(1)` ek alan tüketir (özyineleme yığınına eklenmez, aynı çağrı çerçevesinde çalışır).

Bu nedenle algoritmanın toplam ek alan karmaşıklığı, en büyük bileşen olan özyineleme derinliğine göre:

```text
O(N²)
```

olur. Bu, klasik `isSafe`-tabanlı backtracking sürümüyle aynı alan karmaşıklığıdır; MRV yalnızca zaman/performans üzerinde bir değişiklik yapar, bellek kullanımını artırmaz.