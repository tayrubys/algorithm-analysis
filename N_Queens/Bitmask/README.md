# Bitmask ile Backtracking - N-Queens Problemi 

Bu bölümde N Vezir Probleminin, güvenli kareleri tahtada tek tek dolaşmak yerine **bit maskeleri (bitmask)** üzerinde bitwise işlemlerle takip ederek çözülmesini sağlayan, klasik backtracking yönteminin optimize edilmiş hâli açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** Klasik backtracking çözümünde her kare için `isSafe` fonksiyonuyla satır ve iki çaprazın tek tek (`O(N)` sürede) kontrol edilmesi gerekliliğini ortadan kaldırmak; bunun yerine dolu sütunları ve çaprazları birer tamsayı üzerinde bit maskesi olarak tutup, bir karenin güvenli olup olmadığını **tek bir bitwise işlemle** (`O(1)`) belirlemek için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Klasik backtracking'de her aday kare için ayrı ayrı döngülerle yapılan satır/çapraz kontrolünün getirdiği sabit çarpanı (`O(N)` maliyetli `isSafe`) giderir. Bitmask yaklaşımında dolu sütun, üst çapraz ve alt çapraz bilgileri sırasıyla `cols`, `leftDiag`, `rightDiag` değişkenlerinde tutulur ve tüm satırın güvenli kareleri tek bir `~(cols | leftDiag | rightDiag) & FULL_BOARD` işlemiyle anında hesaplanır.
* **Önceki yöntemlere göre avantajı nedir?** Klasik `isSafe` tabanlı backtracking'e göre, her aday kare için ayrı bir kontrol döngüsü çalıştırmak yerine donanım düzeyinde hızlı bitwise işlemler (`&`, `|`, `~`, `<<`, `>>`) ve `__builtin_ctz` gibi derleyici yerleşik (intrinsic) fonksiyonlar kullanır. Bu, aynı arama ağacını gezerken önemli ölçüde daha az işlem yapılmasını sağlar.
* **Hangi durumda tercih edilir?**
  * `N` değerinin klasik backtracking için bile zorlayıcı olduğu, performansın kritik olduğu durumlarda.
  * Bitwise işlemlerin donanım tarafından hızlı desteklendiği ortamlarda (bit sayısı `N`, işlemci kelime boyutunu aşmadığı sürece).
  * Aynı probleme ait çok sayıda çözümün (veya çözüm sayısının) hızlıca bulunması gereken durumlarda.
* **Hangi durumda tercih edilmez?**
  * `N` değerinin, kullanılan tamsayı türünün bit genişliğini (örneğin 32-bit `int` için `N > 32`) aştığı durumlarda; bu durumda ek büyük tamsayı (bigint/bitset) yapıları gerekir.
  * Kodun okunabilirliğinin ve anlaşılırlığının performanstan daha öncelikli olduğu, eğitim amaçlı veya küçük `N` değerleriyle çalışılan durumlarda; klasik `isSafe` tabanlı backtracking daha anlaşılırdır.
  * `__builtin_ctz` gibi derleyiciye özgü (GCC/Clang) fonksiyonların desteklenmediği taşınabilir (portable) kod gerektiren ortamlarda.

## Algoritmanın Çalışma Mantığı

Klasik yöntemden farklı olarak vezirler **satır satır** yerleştirilir (bir satırda en fazla bir vezir olabileceği zaten garanti edilir). Her satırda, o satırda güvenli olan tüm sütunlar tek bir bitmask işlemiyle anında bulunur.

Her satır için:

1. `cols` maskesindeki set bitler, o ana kadar dolu olan sütunları; `leftDiag` ve `rightDiag` maskeleri ise sırasıyla önceki satırlardan gelen üst-sol ve üst-sağ çapraz tehditleri temsil eder.
2. `safePositions = ~(cols | leftDiag | rightDiag) & FULL_BOARD` işlemiyle, mevcut satırda vezir yerleştirilebilecek tüm sütunlar **tek adımda** hesaplanır.
3. `safePositions` içindeki en düşük anlamlı set bit, `p = safePositions & -safePositions` işlemiyle çıkarılır; bu, denenecek bir sonraki güvenli sütunu temsil eder.
4. `p` bit maskesinden gerçek sütun indeksi, `__builtin_ctz(p)` (en sağdaki set bite kadar olan sıfır sayısı) ile elde edilir.
5. Vezir bu kareye yerleştirilir ve algoritma **özyinelemeli** olarak bir sonraki satır için çağrılır; bu çağrıda `cols` güncellenir, `leftDiag` ve `rightDiag` maskeleri ise sırasıyla sola (`<<1`) ve sağa (`>>1`) kaydırılarak bir sonraki satırdaki çapraz konumlara taşınır.
6. Sonraki satır için çözüm bulunamazsa, az önce yerleştirilen vezir kaldırılır (**backtrack**) ve `safePositions`'taki bir sonraki güvenli sütun denenir.
7. Tüm satırlara (`row == N`) başarıyla vezir yerleştirilirse geçerli bir çözüme ulaşılmış demektir.
8. Bir satırda `safePositions` tükenirse (`0` olursa), o satır için çözüm yoktur; fonksiyon `false` döndürerek bir önceki satıra geri döner.

### Örnek

`N = 4` için, `cols = leftDiag = rightDiag = 0` ile satır `0`'dan başlanır. `FULL_BOARD = (1 << 4) - 1 = 1111` (ikilik tabanda).

```text
Satır 0: safePositions = ~(0000) & 1111 = 1111
         En düşük bit alınır → sütun 0 denenir → sonraki satırlarda çözüm bulunamaz, geri alınır
         Kalan safePositions = 1110 → sütun 1 denenir → yerleştirilir, devam edilir

Satır 1: cols, leftDiag, rightDiag güncellenmiş haliyle
         safePositions hesaplanır → sütun 3 uygun bulunur → yerleştirilir

Satır 2: safePositions hesaplanır → sütun 0 uygun bulunur → yerleştirilir

Satır 3: safePositions hesaplanır → sütun 2 uygun bulunur → yerleştirilir
         row == N (4) → çözüm tamamlandı!
```

Bulunan çözüm:

```text
.  Q  .  .
.  .  .  Q
Q  .  .  .
.  .  Q  .
```

> Bu sonuç, klasik `isSafe` tabanlı backtracking ile bulunan çözümle aynıdır; çünkü her iki yöntem de aynı kısıtları uygular. Fark, güvenli karelerin **nasıl** tespit edildiğidir: klasik yöntemde döngülerle tek tek, bitmask yönteminde ise tek bir bitwise işlemle.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon solveNQBitmaskUtil(row, cols, leftDiag, rightDiag, FULL_BOARD, board):

    // Base case: tüm satırlara vezir yerleştirildiyse
    Eğer row == N ise:
        Doğru döndür

    // Mevcut satırda güvenli olan tüm sütunları tek işlemle hesapla
    safePositions = TAMAMLAYICI(cols VEYA leftDiag VEYA rightDiag) VE FULL_BOARD

    // Denenecek güvenli kare kaldığı sürece döngü çalışır
    Döngü (safePositions > 0):

        // En düşük anlamlı set biti çıkar (en sağdaki güvenli sütun)
        p = safePositions VE (-safePositions)
        safePositions = safePositions - p

        // p bit maskesinden gerçek sütun indeksini bul
        col = CTZ(p)   // en sağdaki set bite kadar olan sıfır sayısı

        // Veziri yerleştir
        board[row][col] = 1

        // Bir sonraki satır için özyinelemeli olarak dene
        // leftDiag sola, rightDiag sağa kaydırılarak bir sonraki satıra taşınır
        Eğer solveNQBitmaskUtil(row + 1, cols VEYA p,
                                  (leftDiag VEYA p) SOLA_KAYDIR 1,
                                  (rightDiag VEYA p) SAGA_KAYDIR 1,
                                  FULL_BOARD, board) == Doğru ise:
            Doğru döndür

        // Sonraki satır için yerleşim mümkün değilse geri al
        board[row][col] = 0

    // Güvenli kare kalmadıysa çözüm yoktur
    Yanlış döndür


Fonksiyon solveNQ():

    board = N x N boyutunda, tamamı 0 olan bir tahta
    FULL_BOARD = (1 SOLA_KAYDIR N) - 1   // N bitin tamamı 1 olan maske

    // Tarama satır 0'dan başlar, tüm maskeler 0 olarak başlatılır
    Eğer solveNQBitmaskUtil(0, 0, 0, 0, FULL_BOARD, board) == Yanlış ise:
        "çözüm yok" yazdır
        Yanlış döndür

    Tahtayı yazdır
    Doğru döndür
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`#define N 4`** — Tahtanın boyutunu ve yerleştirilecek vezir sayısını belirleyen bir makrodur.

* **`int board[N][N]`** — Bulunan çözümün yazdırılabilmesi için vezir konumlarını tutan iki boyutlu dizidir. Arama sürecinin kendisi bu diziye değil, bit maskelerine dayanır; `board` yalnızca sonucu görselleştirmek için güncellenir.

* **`int cols`** — O ana kadar dolu olan sütunları bir bit maskesi olarak tutar. `col` sütunundaki bit `1` ise o sütun doludur.

* **`int leftDiag`, `int rightDiag`** — Sırasıyla üst-sol ve üst-sağ çapraz tehditleri temsil eden bit maskeleridir. Her satır geçişinde sırasıyla `<< 1` (sola kaydırma) ve `>> 1` (sağa kaydırma) ile bir sonraki satırdaki karşılık gelen çapraz konumlara kaydırılırlar.

* **`int FULL_BOARD = (1 << N) - 1`** — En düşük `N` biti `1`, geri kalanı `0` olan bir maskedir; geçerli sütun aralığını (`0..N-1`) temsil eder ve hesaplamaların tahta sınırları dışına taşmasını önler.

* **`safePositions = ~(cols | leftDiag | rightDiag) & FULL_BOARD`** — Mevcut satırda vezir yerleştirilebilecek tüm sütunları tek bir işlemde hesaplayan temel bitwise ifadedir.

* **`p = safePositions & -safePositions`** — İkinin tümleyeni (two's complement) özelliğinden yararlanarak bir sayıdaki **en düşük anlamlı set biti** izole eden klasik bit hilesidir (`-safePositions`, `safePositions`'ın tüm bitlerinin tersini alıp `1` ekler).

* **`__builtin_ctz(p)`** — "Count Trailing Zeros" (sondaki sıfırları say) anlamına gelen, GCC/Clang'e özgü bir yerleşik (intrinsic) fonksiyondur. `p` yalnızca tek bir bit içerdiği için, bu fonksiyon o bitin (yani sütunun) indeksini doğrudan verir.

* **`(leftDiag | p) << 1`, `(rightDiag | p) >> 1`** — Yeni yerleştirilen vezirin çapraz etkisini bir sonraki satıra taşıyan kaydırma işlemleridir; bu sayede çapraz kontrolü için ayrı bir döngüye gerek kalmaz.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
```

* `<iostream>`: `cout` kullanılarak tahtanın ve çözüm durumunun ekrana yazdırılması için kullanılır.

> Bitmask yaklaşımı ek bir kütüphane gerektirmez; `__builtin_ctz` gibi fonksiyonlar derleyici (GCC/Clang) tarafından yerleşik olarak sağlanır ve ayrı bir başlık dosyası (`#include`) gerektirmez.

---

## Zaman Karmaşıklığı

`solveNQBitmaskUtil` fonksiyonu her satır için, `safePositions` içindeki set bit sayısı kadar (en kötü durumda `N`) aday sütun dener:

```cpp
while (safePositions > 0)
```

Klasik yöntemden farklı olarak, her aday için ayrı bir `O(N)` maliyetli `isSafe` kontrolü **yoktur**. Güvenli sütunların tamamı, satır başına tek bir bitwise işlemle (`O(1)`, kelime boyutu sabit kabul edilirse) hesaplanır. Bu nedenle her düğümdeki (node) işlem maliyeti klasik yönteme göre önemli ölçüde azalır.

### Best Case — O(N)

En iyi durumda, her satırda denenen ilk sütun doğrudan güvenli çıkar ve hiç geri alma yapılmadan çözüme ulaşılır. Her satırda sabit sayıda bitwise işlem yapıldığı için en iyi durum zaman karmaşıklığı yaklaşık olarak:

```text
O(N)
```

olur.

### Average Case — Üstel (Sub-factorial), Daha Düşük Sabit Çarpanla

Ortalama durumda, algoritma bazı satırlarda geri alma yapar; ancak her düğümdeki işlem maliyeti `O(1)`'e yakın olduğu için, klasik `O(N)` maliyetli `isSafe` kontrolüne kıyasla toplam işlem sayısı önemli ölçüde azalır. Arama ağacının şekli (kaç düğüm gezildiği) klasik yöntemle aynı kalır; fark, düğüm başına harcanan sürededir.

### Worst Case — O(N!)

En kötü durumda (çözümün geç bulunduğu veya bulunamadığı durumlarda), algoritma arama ağacının büyük bir kısmını gezmek zorunda kalır. Gezilen düğüm sayısı asimptotik olarak klasik backtracking ile aynı üst sınıra, yani:

```text
O(N!)
```

mertebesine sahiptir. Bitmask optimizasyonu bu üst sınırı değiştirmez; yalnızca her düğümdeki sabit çarpanı (`isSafe`'in `O(N)` maliyetini `O(1)`'e indirerek) küçültür, bu da pratikte önemli bir hız artışı sağlar.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N²)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada bulunan çözümü saklamak için `N × N` boyutunda iki boyutlu bir dizi kullanılmıştır:

```cpp
int board[N][N];
```

Bu, algoritmanın en büyük ek bellek kullanımını oluşturur ve `O(N²)` alan tüketir.

`cols`, `leftDiag`, `rightDiag` ve `FULL_BOARD` gibi bit maskeleri, `N` bitlik bilgiyi tek bir tamsayı içinde sakladığı için `O(1)` alan tüketir — bu, klasik yöntemdeki `O(N)` boyutlu ek dizilere (örneğin dolu sütunları/çaprazları ayrı ayrı takip eden diziler) kıyasla önemli bir bellek avantajıdır.

`solveNQBitmaskUtil` fonksiyonu özyinelemeli olarak çağrıldığı için, çağrı yığını (call stack) en fazla `N` derinliğe ulaşır ve `O(N)` ek alan tüketir.

Bu nedenle algoritmanın toplam ek alan karmaşıklığı, en büyük bileşen olan tahta boyutuna göre:

```text
O(N²)
```

olur.