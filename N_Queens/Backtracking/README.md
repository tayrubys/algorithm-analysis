# Backtracking Algoritması — N-Queens Problemi

Bu bölümde N Vezir Probleminin, vezirleri sütun sütun yerleştirip her yerleştirmenin geçerliliğini kontrol ederek, geçersiz bir duruma düşüldüğünde bir önceki adıma geri dönerek (**backtracking**) çözülmesini sağlayan yöntem açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** `N × N` boyutundaki bir satranç tahtasına, birbirini tehdit etmeyecek şekilde `N` adet vezir yerleştirme problemini çözmek için geliştirilmiştir. Bütün olası yerleşimleri (`N^N` kombinasyon) sonuna kadar üretip kontrol etmek yerine, geçersiz bir yerleşime ulaşıldığı anda o dalı terk ederek arama uzayını daraltmayı amaçlar.
* **Hangi eksikliği gidermektedir?** Tüm olası vezir yerleşimlerini tek tek deneyip geçerliliğini en sonda kontrol eden kaba kuvvet (brute force) yaklaşımının yarattığı büyük zaman kaybını giderir. Backtracking, bir vezir yerleştirildiği anda geçersizlik tespit edilirse, o sütundan sonrasını hiç denemeden geri döner.
* **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, geçersiz bir yerleşime erken aşamada karar vererek (**erken budama / pruning**) aramayı büyük ölçüde hızlandırmasıdır. Bütün `N^N` olasılığı denemek yerine, yalnızca güvenli (safe) olan dallar üzerinde ilerler.
* **Hangi durumda tercih edilir?**
  * Kesin ve geçerli bir çözümün (veya çözümün var olmadığının) garanti olarak bulunması gerektiği durumlarda.
  * Problemin doğası gereği, bir kısmi çözümün geçersizliğinin erken aşamada anlaşılabildiği (kısıt tabanlı) problemlerde.
  * `N` değerinin makul (küçük-orta) boyutlarda olduğu durumlarda.
* **Hangi durumda tercih edilmez?**
  * `N` değerinin çok büyük olduğu durumlarda. Backtracking budama yapsa da en kötü durumda üstel (`O(N!)` mertebesinde) bir zaman karmaşıklığına sahiptir; bu nedenle çok büyük tahtalarda pratik değildir.
  * Yalnızca yaklaşık veya "yeterince iyi" bir sonucun yeterli olduğu, kesin çözüm zorunluluğunun olmadığı durumlarda (bu gibi durumlarda sezgisel/heuristik yöntemler tercih edilebilir).

## Algoritmanın Çalışma Mantığı

Vezirler sütun sütun (soldan sağa) yerleştirilir. Her sütun için, o sütundaki her satır tek tek denenir; bir vezir yerleştirilmeden önce mevcut tahtaya göre güvenli olup olmadığı kontrol edilir.

Her sütun için:

1. Mevcut sütundaki satırlar `0`'dan `N-1`'e kadar tek tek denenir.
2. Denenen `(satır, sütun)` karesine vezir yerleştirmenin güvenli olup olmadığı kontrol edilir: aynı satırda solda başka bir vezir var mı, üst-sol çaprazda vezir var mı, alt-sol çaprazda vezir var mı (sağ taraf henüz doldurulmadığı için kontrol edilmez).
3. Kare güvenliyse vezir oraya yerleştirilir ve algoritma **özyinelemeli (recursive)** olarak bir sonraki sütun için çağrılır.
4. Sonraki sütun için bir çözüm bulunamazsa, az önce yerleştirilen vezir tahtadan kaldırılır (**geri alma / backtrack**) ve aynı sütunda bir sonraki satır denenir.
5. Bütün sütunlar (`col == N`) başarıyla dolduruluysa geçerli bir çözüme ulaşılmış demektir ve `true` döndürülür.
6. Bir sütunda hiçbir satır güvenli değilse, o sütun için çözüm yoktur ve fonksiyon `false` döndürerek bir önceki sütuna geri döner.

### Örnek

`N = 4` için boş bir tahta ile başlanır ve vezirler sütun sütun yerleştirilir. Algoritmanın bulduğu ilk geçerli çözüm şu şekildedir:

```text
.  Q  .  .
.  .  .  Q
Q  .  .  .
.  .  Q  .
```

Yerleştirme süreci özetle şöyle ilerler:

```text
Sütun 0: satır 0 denenir → güvenli, yerleştirilir → sonraki sütunda çözüm bulunamaz → geri alınır
         satır 1 denenir → güvenli değil (çapraz/satır çakışması)
         satır 2 denenir → güvenli, yerleştirilir → sonraki sütunlarla devam edilir

Sütun 1: satır 0 denenir → güvenli, yerleştirilir → devam edilir

Sütun 2: uygun satır bulunamaz → geri alınır, sütun 1'de bir sonraki satır denenir

... (bu geri alma süreci tüm sütunlarda geçerli bir yerleşim bulunana kadar sürer)

Sonuç: (satır, sütun) çiftleri → (2,0), (0,1), (3,2), (1,3)
```

Algoritma, `col >= N` koşuluna ulaştığında (yani 4 sütunun tamamına geçerli şekilde vezir yerleştirildiğinde) çözümü bulmuş olur.

> Backtracking, bir vezirin geçersiz olduğunu keşfettiği anda o dalı tamamen terk eder. Sonraki sütunlar hiç denenmeden geriye dönülür; bu da bütün `N^N` olasılığın denenmesini engeller.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon isSafe(board, row, col):

    // Sol satır kontrolü (aynı satırda başka vezir var mı)
    Döngü (i = 0'dan col - 1'e kadar):
        Eğer board[row][i] == 1 ise:
            Yanlış döndür

    // Üst çapraz kontrolü
    Döngü (i = row, j = col; i >= 0 VE j >= 0; i azalt, j azalt):
        Eğer board[i][j] == 1 ise:
            Yanlış döndür

    // Alt çapraz kontrolü
    Döngü (i = row, j = col; j >= 0 VE i < N; i artır, j azalt):
        Eğer board[i][j] == 1 ise:
            Yanlış döndür

    Doğru döndür


Fonksiyon solveNQUtil(board, col):

    // Base case: tüm sütunlara vezir yerleştirildiyse
    Eğer col >= N ise:
        Doğru döndür

    Döngü (i = 0'dan N - 1'e kadar):

        // Vezir bu kareye yerleştirilebilir mi kontrol et
        Eğer isSafe(board, i, col) ise:

            // Veziri yerleştir
            board[i][col] = 1

            // Bir sonraki sütun için özyinelemeli olarak dene
            Eğer solveNQUtil(board, col + 1) == Doğru ise:
                Doğru döndür

            // Sonraki sütun için yerleşim mümkün değilse geri al
            board[i][col] = 0

    Yanlış döndür


Fonksiyon solveNQ():

    board = N x N boyutunda, tamamı 0 olan bir tahta

    Eğer solveNQUtil(board, 0) == Yanlış ise:
        "çözüm yok" yazdır
        Yanlış döndür

    Tahtayı yazdır
    Doğru döndür
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`#define N 4`** — Tahtanın boyutunu (satır/sütun sayısını) ve yerleştirilecek vezir sayısını belirleyen bir makrodur. Derleme zamanında sabit bir değere genişletilir.

* **`int board[N][N]`** — `N × N` boyutunda iki boyutlu bir dizidir. `board[i][j] == 1` ise `(i, j)` karesinde bir vezir bulunduğunu, `0` ise boş olduğunu ifade eder.

* **`isSafe(board, row, col)`** — Belirli bir `(row, col)` karesine vezir yerleştirmenin güvenli olup olmadığını kontrol eden yardımcı fonksiyondur. Yalnızca sol tarafı kontrol eder çünkü sütunlar soldan sağa dolduğu için sağ taraf henüz boştur.

* **`solveNQUtil(board, col)`** — Vezirleri sütun sütun yerleştiren, **özyinelemeli (recursive)** backtracking fonksiyonudur. Başarısız bir yerleşimde `board[i][col] = 0;` satırıyla veziri tahtadan kaldırarak bir önceki duruma geri döner.

* **`i, j` döngü değişkenleri** — `isSafe` fonksiyonunda satır ve çapraz kontrollerinde kullanılan sayaç değişkenleridir; çapraz kontrollerde aynı anda hem artan hem azalan yönde ilerletilirler (`i--, j--` / `i++, j--`).

* **`printSolution(board)`** — Bulunan çözümü ekrana yazdırır; `board[i][j]` değeri `1` ise `" Q "`, değilse `" . "` basar.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
```

* `<iostream>`: `cout` kullanılarak tahtanın ve çözüm durumunun ekrana yazdırılması için kullanılır.

> Kodda `<vector>` gibi dinamik veri yapıları kullanılmamıştır; tahta boyutu `#define N` ile derleme zamanında sabitlendiği için sabit boyutlu bir diziyle (`int board[N][N]`) çalışılmıştır.

---

## Zaman Karmaşıklığı

`solveNQUtil` fonksiyonu her sütun için `N` satırı dener:

```cpp
for (int i = 0; i < N; i++)
```

ve her satır denemesinde `isSafe` fonksiyonu çağrılır. `isSafe`, en kötü durumda `O(N)` sürede çalışır (satır ve iki çapraz kontrolü).

Backtracking olmadan, kaba kuvvetle bütün olasılıkları denemek `O(N^N)` sürede çalışırdı. Ancak `isSafe` kontrolü her adımda geçersiz dalları erken budadığı için, arama uzayı önemli ölçüde daralır.

### Best Case — O(N²)

En iyi durumda, her sütun için denenen ilk birkaç satır doğrudan güvenli çıkar ve hiç geri alma (backtrack) yapılmadan çözüme ulaşılır. Bu durumda her sütun için yaklaşık `O(N)` sürede bir satır denenir ve `isSafe` kontrolü de `O(N)` sürer. Bu nedenle en iyi durum zaman karmaşıklığı yaklaşık olarak:

```text
O(N²)
```

olur.

### Average Case — Üstel (Sub-factorial)

Ortalama durumda, algoritma bazı sütunlarda geçersiz yerleşimlerle karşılaşır ve geri alma yaparak farklı satırları dener. Denenen dal sayısı, tahtanın boyutuna ve budama etkinliğine bağlıdır. Bu nedenle ortalama durum karmaşıklığı, en kötü durumdan daha iyi olsa da genel olarak üstel mertebede kabul edilir.

### Worst Case — O(N!)

En kötü durumda (özellikle çözümün bulunmadığı veya çok geç bulunduğu `N` değerlerinde), algoritma arama ağacının büyük bir kısmını gezmek zorunda kalır. Klasik N-Queens backtracking çözümünün en kötü durum zaman karmaşıklığı, literatürde yaklaşık olarak:

```text
O(N!)
```

şeklinde kabul edilir. Bu, bütün `N^N` olasılığı denemekten (kaba kuvvet) çok daha iyi olsa da, `N` büyüdükçe hâlâ hızla büyüyen bir karmaşıklıktır.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N²)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada tahtayı saklamak için `N × N` boyutunda iki boyutlu bir dizi kullanılmıştır:

```cpp
int board[N][N];
```

Bu, algoritmanın en büyük ek bellek kullanımını oluşturur ve `O(N²)` alan tüketir.

`solveNQUtil` fonksiyonu özyinelemeli (recursive) olarak çağrıldığı için, çağrı yığını (call stack) en fazla `N` derinliğe ulaşır (her sütun için bir özyinelemeli çağrı). Bu nedenle özyineleme yığını `O(N)` ek alan tüketir.

`row`, `col`, `i`, `j` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Bu nedenle algoritmanın toplam ek alan karmaşıklığı, en büyük bileşen olan tahta boyutuna göre:

```text
O(N²)
```

olur.