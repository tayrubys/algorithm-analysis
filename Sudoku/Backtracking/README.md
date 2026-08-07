# Backtracking Algoritması — Sudoku Problemi

Bu bölümde Sudoku bulmacasının, boş hücreleri sırayla dolaşıp her sayı denemesinin geçerliliğini kontrol ederek, geçersiz bir duruma düşüldüğünde bir önceki adıma geri dönerek (**backtracking**) çözülmesini sağlayan yöntem açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** `9 × 9` boyutundaki bir Sudoku tahtasındaki boş hücrelere, satır/sütun/`3 × 3` kutu kısıtlarını ihlal etmeyecek şekilde `1`'den `9`'a kadar sayılar yerleştirme problemini çözmek için geliştirilmiştir. Bütün olası sayı kombinasyonlarını sonuna kadar üretip en sonda geçerliliğini kontrol etmek yerine, geçersiz bir yerleşime ulaşıldığı anda o dalı terk ederek arama uzayını daraltmayı amaçlar.
* **Hangi eksikliği gidermektedir?** Her boş hücreye rastgele veya sırayla sayı deneyip bulmacanın tamamı doldurulduktan sonra geçerliliği kontrol eden kaba kuvvet (brute force) yaklaşımının yarattığı büyük zaman kaybını giderir. Backtracking, bir sayı yerleştirildiği anda geçersizlik tespit edilirse, kalan hücreleri hiç doldurmadan geri döner.
* **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, geçersiz bir yerleşime erken aşamada karar vererek (**erken budama / pruning**) aramayı büyük ölçüde hızlandırmasıdır. Her hücreye sayı yerleştirilmeden önce `isSafe` kontrolüyle geçerliliği anında sınanır; geçersiz dallar hiç derinlemesine gezilmez.
* **Hangi durumda tercih edilir?**
  * Kesin ve geçerli bir çözümün (veya çözümün var olmadığının) garanti olarak bulunması gerektiği durumlarda.
  * Bulmacanın başlangıçta yeterince dolu (ipucu sayısı yüksek) olduğu, dolayısıyla arama uzayının pratikte hızlı daraldığı durumlarda.
  * Standart `9 × 9` Sudoku gibi makul boyutlu bulmacalarda.
* **Hangi durumda tercih edilmez?**
  * Bulmacanın neredeyse tamamen boş olduğu veya çok az ipucu içerdiği, dolayısıyla arama ağacının çok büyük olduğu durumlarda; bu gibi zorlayıcı durumlarda saf backtracking yavaş kalabilir ve ek sezgisel yöntemlere (örneğin "en kısıtlı hücreyi önce doldur" gibi) ihtiyaç duyulabilir.
  * Yalnızca yaklaşık veya kısmi bir çözümün yeterli olduğu, kesin çözüm zorunluluğunun olmadığı durumlarda.

## Algoritmanın Çalışma Mantığı

Hücreler soldan sağa, yukarıdan aşağıya doğru sırayla (`row`, `col`) dolaşılır. Her boş hücre için `1`'den `9`'a kadar sayılar tek tek denenir; bir sayı yerleştirilmeden önce mevcut tahtaya göre geçerli olup olmadığı kontrol edilir.

Her hücre için:

1. Satırın sonuna ulaşıldıysa (`col == n`), bir sonraki satıra geçilir (`row++`, `col = 0`).
2. Son satırın son sütununa ulaşıldıysa (`row == n - 1 && col == n`), bulmaca tamamen çözülmüş demektir.
3. Mevcut hücre zaten doluysa (girdi bulmacasının bir parçasıysa), hiçbir şey denenmeden doğrudan bir sonraki hücreye geçilir.
4. Hücre boşsa, `1`'den `9`'a kadar her sayı için, bu sayının aynı satırda, aynı sütunda veya bulunduğu `3 × 3` kutuda zaten var olup olmadığı kontrol edilir (`isSafe`).
5. Sayı geçerliyse hücreye yerleştirilir ve algoritma **özyinelemeli (recursive)** olarak bir sonraki hücre için çağrılır.
6. Sonraki hücreler için bir çözüm bulunamazsa, az önce yerleştirilen sayı hücreden kaldırılır (**geri alma / backtrack**, hücre tekrar `0` yapılır) ve bir sonraki sayı denenir.
7. Bir hücrede `1`'den `9`'a kadar hiçbir sayı geçerli olmuyorsa, o hücre için çözüm yoktur ve fonksiyon `false` döndürerek bir önceki hücreye geri döner.

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

Algoritma `(0,0)` hücresinden başlayarak ilerler:

```text
(0,0): zaten dolu (3) → doğrudan (0,1)'e geç
(0,1): boş → 1 denenir → isSafe(1,1'de) satırda/sütunda/kutuda çakışma yok → yerleştirilir
(0,4): boş → sırasıyla 1,2,3... denenir → 7 geçerli bulunur → yerleştirilir
...
Bir noktada hiçbir sayı bir hücreye sığmazsa → geri dönülür,
bir önceki hücrede denenen sayı değiştirilir ve arama devam eder.
```

Algoritma tüm hücreleri geçerli şekilde doldurduğunda aşağıdaki çözüme ulaşır:

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

> Backtracking, bir sayının geçersiz olduğunu keşfettiği anda o dalı tamamen terk eder. Kalan hücreler hiç denenmeden geriye dönülür; bu da bütün `9^(boş hücre sayısı)` olasılığın denenmesini engeller.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon isSafe(mat, row, col, num):

    // Sayının satırda olup olmadığını kontrol et
    Döngü (x = 0'dan 8'e kadar):
        Eğer mat[row][x] == num ise:
            Yanlış döndür

    // Sayının sütunda olup olmadığını kontrol et
    Döngü (x = 0'dan 8'e kadar):
        Eğer mat[x][col] == num ise:
            Yanlış döndür

    // Sayının bulunduğu 3x3'lük kutuda olup olmadığını kontrol et
    startRow = row - (row mod 3)
    startCol = col - (col mod 3)

    Döngü (i = 0'dan 2'ye kadar):
        Döngü (j = 0'dan 2'ye kadar):
            Eğer mat[i + startRow][j + startCol] == num ise:
                Yanlış döndür

    Doğru döndür


Fonksiyon solveSudokuRec(mat, row, col):

    n = mat boyutu

    // Base case: son satırın son sütununa ulaşıldıysa Sudoku çözülmüştür
    Eğer row == n - 1 VE col == n ise:
        Doğru döndür

    // Satırın son sütununa gelindiyse bir sonraki satıra geç
    Eğer col == n ise:
        row = row + 1
        col = 0

    // Hücre zaten doluysa bir sonraki hücreye geç
    Eğer mat[row][col] != 0 ise:
        Döndür solveSudokuRec(mat, row, col + 1)

    Döngü (num = 1'den n'e kadar):

        // Sayıyı mevcut konuma yerleştirmek güvenliyse
        Eğer isSafe(mat, row, col, num) ise:

            mat[row][col] = num

            // Bir sonraki hücreyi çözmeye çalış
            Eğer solveSudokuRec(mat, row, col + 1) == Doğru ise:
                Doğru döndür

            // Başarısızsa geri al
            mat[row][col] = 0

    Yanlış döndür


Fonksiyon solveSudoku(mat):

    solveSudokuRec(mat, 0, 0)
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<vector<int>> &mat`** — Sudoku tahtasını iki boyutlu bir vektör olarak referans ile tutar. `mat[i][j] == 0` ise ilgili hücre boş, aksi hâlde `1`-`9` arası bir sayı ile dolu demektir. Referans (`&`) kullanımı, tahtanın kopyalanmadan doğrudan üzerinde değişiklik yapılmasını sağlar.

* **`isSafe(mat, row, col, num)`** — Belirli bir `(row, col)` hücresine `num` sayısını yerleştirmenin geçerli olup olmadığını üç ayrı kontrolle (satır, sütun, `3×3` kutu) sınayan yardımcı fonksiyondur.

* **`int startRow`, `int startCol`** — Mevcut hücrenin ait olduğu `3×3` kutunun sol-üst köşesinin koordinatlarını hesaplar (`row - (row % 3)`, `col - (col % 3)`); kutu kontrolü bu köşeden başlayarak yapılır.

* **`solveSudokuRec(mat, row, col)`** — Hücreleri sırayla dolduran, **özyinelemeli (recursive)** backtracking fonksiyonudur. Başarısız bir yerleşimde `mat[row][col] = 0;` satırıyla sayıyı hücreden kaldırarak bir önceki duruma geri döner. Dolu hücrelerde sınama yapmadan bir sonraki hücreye geçer.

* **`col == n` kontrolü ve satır atlaması** — İki boyutlu tahtayı tek bir doğrusal özyineleme zinciriyle (`col`'u sürekli artırarak) dolaşmayı sağlayan mantıktır; sütun sonuna gelindiğinde `row` artırılıp `col` sıfırlanarak bir sonraki satıra geçilir.

* **`mat.size()`** — Tahtanın boyutunu (`n = 9`) döndürmek için kullanılır.

> Not: `isSafe` fonksiyonundaki satır/sütun kontrol döngüleri (`for (int x = 0; x <= 8; x++)`) doğrudan `8` sabitine göre yazılmıştır; `solveSudokuRec` içindeki `n = mat.size()` genel boyutu kullansa da, `isSafe` standart `9×9` Sudoku'ya göre sabitlenmiştir.

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

`solveSudokuRec` fonksiyonu, her boş hücre için `1`'den `9`'a kadar (`n` adet) sayıyı sırayla dener:

```cpp
for (int num = 1; num <= n; num++)
```

Her sayı denemesinde `isSafe` fonksiyonu çağrılır. `isSafe`, satır kontrolü (`O(N)`), sütun kontrolü (`O(N)`) ve `3 × 3` kutu kontrolü (`O(1)`) yaptığı için toplam çalışma süresi `O(N)`'dir.

Backtracking sayesinde geçersiz yerleşimler erken tespit edilir ve bu dallar daha fazla araştırılmadan terk edilir (pruning). Bu durum pratikte algoritmayı oldukça hızlandırsa da teorik en kötü durum zaman karmaşıklığını değiştirmez.

### Best Case — O(N²)

En iyi durumda Sudoku zaten çözülmüş ya da çok az boş hücre içeriyordur. Algoritma hücreleri yalnızca bir kez dolaşır ve neredeyse hiç geri alma (backtracking) işlemi yapmaz. Dolu hücrelerde `isSafe` fonksiyonu çağrılmadan doğrudan bir sonraki hücreye geçilir.

Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N²)
```

şeklindedir.

### Average Case — Üstel (Exponential)

Ortalama durumda algoritma bazı hücrelerde yanlış seçimler yapar ve geri alma (backtracking) işlemi gerçekleştirir. Çalışma süresi bulmacadaki boş hücre sayısına ve başlangıç yerleşimine bağlıdır. Bu nedenle ortalama durum için kesin bir Big-O değeri vermek mümkün değildir; ancak genel olarak üstel (exponential) davranış gösterdiği kabul edilir.

### Worst Case — O(9^k)

En kötü durumda algoritma, her boş hücre için `9` farklı sayıyı denemek zorunda kalabilir.

Boş hücre sayısı `k` ise en kötü durum zaman karmaşıklığı:

```text
O(9^k)
```

şeklindedir.

Standart `9 × 9` Sudoku'da en fazla `81` hücre boş olabileceğinden (`k = N²`), bu ifade en kötü durumda:

```text
O(9^(N²))
```

olarak da yazılabilir.

Bu, backtracking tabanlı Sudoku çözücüsünün teorik üst sınırıdır. Gerçek uygulamalarda ise `isSafe` kontrolü sayesinde çok sayıda geçersiz dal erken elendiğinden algoritma çoğu Sudoku bulmacasını bu teorik sınırdan çok daha kısa sürede çözer.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N²)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

`mat`, fonksiyona dışarıdan (girdi olarak) referans ile aktarıldığı ve algoritma tarafından yeniden oluşturulmadığı için, kendisi ek bellek kullanımına dahil edilmez; yalnızca üzerinde yerinde (in-place) değişiklik yapılır.

`solveSudokuRec` fonksiyonu özyinelemeli olarak çağrıldığı için, çağrı yığını (call stack) en fazla tahtadaki hücre sayısı kadar (`N × N`) derinliğe ulaşabilir; her hücre için bir özyinelemeli çağrı yapılır. Bu nedenle özyineleme yığını `O(N²)` ek alan tüketir.

`row`, `col`, `num`, `n`, `startRow`, `startCol` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Bu nedenle algoritmanın toplam ek alan karmaşıklığı, en büyük bileşen olan özyineleme derinliğine göre:

```text
O(N²)
```

olur.