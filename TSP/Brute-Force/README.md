# TSP Brute Force Algoritması

Bu bölümde Gezgin Satıcı Probleminin tüm olası rotaları deneyerek çözülmesini sağlayan Brute Force yöntemi açıklanmaktadır.

### Algoritmanın Geliştirilme Amacı

Kaba Kuvvet (Brute Force) algoritması, problemi çözmek için en sezgisel ve en temel yaklaşımdır. Geliştirilme amacı hızı veya verimliliği artırmak değil, problemin **kesin ve en iyi (optimal)** çözümünü %100 garanti altına almaktır.

* **Hangi eksikliği gidermektedir?** Gelişmiş sezgisel (heuristic) veya yaklaşık (approximation) algoritmalar her zaman en iyi sonucu garanti etmez. Brute Force, tüm olasılıkları denediği için "Acaba daha kısa bir yol var mıydı?" şüphesini tamamen ortadan kaldırır.
* **Önceki yöntemlere göre avantajı nedir?** Uygulanması, kodlanması ve çalışma mantığının anlaşılması en kolay yöntemdir. Karmaşık veri yapıları gerektirmez.
* **Hangi durumda tercih edilir?** Şehir sayısının ($N$) çok az olduğu (örneğin $N \le 12$) küçük veri setlerinde veya diğer karmaşık algoritmaların ürettiği sonuçların doğruluğunu test etmek için bir "referans (baseline)" noktası oluşturulmak istendiğinde tercih edilir.
* **Hangi durumda tercih edilmez?** Faktöriyel büyüme oranına sahip olduğu için, şehir sayısı arttığında (örneğin $N > 15$) modern bilgisayarların bile hesaplaması yıllar süreceğinden büyük çaplı veri setlerinde ve gerçek zamanlı sistemlerde kesinlikle tercih edilmez.

### Algoritmanın Çalışma Mantığı (Temel Fikir ve Prensip)

Algoritmanın çalışma prensibi, "mümkün olan tüm seçenekleri üret ve en iyisini seç" mantığına dayanır. Adım adım çalışma prensibi şu şekildedir:

1. Sabit bir başlangıç şehri belirlenir.
2. Başlangıç şehri dışındaki tüm şehirlerin olasılık dahilindeki bütün sıralamaları (permütasyonları) teker teker oluşturulur.
3. Her bir permütasyon için, şehirlere sırasıyla uğranarak rotanın toplam mesafe maliyeti toplanır.
4. Son şehre gelindiğinde, tekrar başlangıç şehrine dönmek için gereken mesafe de toplama eklenerek turun maliyeti tamamlanır.
5. Her hesaplanan tur maliyeti, o ana kadar bulunmuş olan en düşük (minimum) tur maliyeti ile karşılaştırılır; daha küçükse güncellenir.
6. Tüm permütasyonlar tükendiğinde, elde kalan minimum değer en iyi rotadır.

### Sözde Kod (Pseudo-Code)

```text
Fonksiyon TSP_BruteForce(Graf, BaslangicSehri):
    // Başlangıç şehri dışındaki şehirleri bir listeye al
    KalanSehirler = BaslangicSehri hariç tüm düğümler
    
    // Minimum maliyeti olabilecek en yüksek değere eşitle
    MinMaliyet = SONSUZ 
    
    // Kalan şehirlerin tüm olası dizilimlerini (permütasyonlarını) üret
    Döngü (KalanSehirler'in tüm permütasyonları varken):
        MevcutMaliyet = 0
        SuAnkiSehir = BaslangicSehri
        
        // Üretilen dizilimdeki şehirlere sırayla git ve maliyeti topla
        İç Döngü (Sehir = KalanSehirler'deki sıradaki şehir):
            MevcutMaliyet = MevcutMaliyet + Graf[SuAnkiSehir][Sehir]
            SuAnkiSehir = Sehir
            
        // Son şehirden başlangıç şehrine geri dön
        MevcutMaliyet = MevcutMaliyet + Graf[SuAnkiSehir][BaslangicSehri]
        
        // Daha iyi bir rota bulduysak minimum maliyeti güncelle
        Eğer MevcutMaliyet < MinMaliyet ise:
            MinMaliyet = MevcutMaliyet
            
    // En iyi rotanın maliyetini döndür
    Döndür MinMaliyet

```

## 2. Algoritmanın Çalışma Mantığı
 
### Temel Fikir
 
Başlangıç şehri sabit tutulur. Başlangıç şehri dışındaki şehirlerin bütün olası sıralamaları oluşturulur.
 
Her sıralama için:
 
1. Başlangıç şehrinden yola çıkılır.
2. Şehirler sıralamaya göre ziyaret edilir.
3. Şehirler arasındaki maliyetler toplanır.
4. Son şehirden başlangıç şehrine dönüş maliyeti eklenir.
5. Hesaplanan değer mevcut minimum maliyetle karşılaştırılır.
6. Daha düşük bir maliyet bulunursa minimum maliyet güncellenir.
Örneğin şehirler `0, 1, 2, 3` ve başlangıç şehri `0` ise kontrol edilen rotalar şunlardır:
 
```
0 → 1 → 2 → 3 → 0
0 → 1 → 3 → 2 → 0
0 → 2 → 1 → 3 → 0
0 → 2 → 3 → 1 → 0
0 → 3 → 1 → 2 → 0
0 → 3 → 2 → 1 → 0
```
 
Bütün rotaların maliyeti hesaplandıktan sonra en küçük maliyet sonuç olarak döndürülür.
 
---
 
### Kullanılan Veri Yapıları ve STL Özellikleri
 
### `vector<vector<int>>`
 
Şehirler arasındaki maliyetleri matris şeklinde saklamak için kullanılır.
 
```
graph[i][j]
```
 
Bu ifade, `i` numaralı şehirden `j` numaralı şehre gitmenin maliyetini gösterir.
 
### `vector<int>`
 
Başlangıç şehri dışındaki şehirleri saklamak için kullanılır.
 
### `next_permutation`
 
Şehirlerin bütün olası sıralamalarını oluşturmak için kullanılır.
 
```
next_permutation(nodes.begin(), nodes.end())
```
 
### `INT_MAX`
 
Minimum maliyet değişkenine başlangıçta mümkün olan en büyük tam sayı değerini vermek için kullanılır.
 
---
 
### Kullanılan Kütüphaneler
 
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
```
 
* `<iostream>`: Ekrana çıktı yazdırmak için kullanılır.
* `<vector>`: Maliyet matrisini ve şehir listesini saklamak için kullanılır.
* `<algorithm>`: `next_permutation` fonksiyonunu kullanmak için eklenir.
* `<climits>`: `INT_MAX` değerini kullanmak için eklenir.

### Zaman Karmaşıklığı
 
Başlangıç şehri sabit tutulduğu için diğer `N - 1` şehrin bütün sıralamaları oluşturulur.
 
**Toplam permütasyon sayısı:**
 
```
(N - 1)!
```
 
Her permütasyonun rota maliyetini hesaplamak için yaklaşık `N` şehir dolaşılır.
 
Bu nedenle toplam zaman karmaşıklığı:
 
```
O(N × (N - 1)!)
```
 
olur. Bu ifade genel olarak:
 
```
O(N!)
```
 
şeklinde gösterilir.
 
### Best Case — O(N!)
Brute Force algoritması en kısa rotayı erken bulsa bile bunun gerçekten en kısa rota olduğunu doğrulamak için diğer bütün rotaları kontrol eder.
 
### Average Case — O(N!)
Ortalama durumda da bütün permütasyonlar oluşturulur ve maliyetleri hesaplanır.
 
### Worst Case — O(N!)
En kötü durumda da bütün olası rotalar kontrol edilir. Bu nedenle algoritmanın en iyi, ortalama ve en kötü durum karmaşıklıkları faktöriyeldir.
 
---

### Alan Karmaşıklığı

 #### Alan karmaşıklığı: O(N)
 
 #### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi:
 Kullanılan veri yapıları temel düzeydedir. Rotayı oluştururken başlangıç şehri dışındaki şehirlerin listesini tutmak için $N-1$ elemanlı tek boyutlu bir dizi (vektör) kullanılmıştır. Permütasyon üretme işlemi ekstra bellek tahsis etmeden, mevcut dizi üzerinde yer değiştirmeler (in-place swap) yaparak gerçekleştirilir (C++ std::next_permutation yapısı). Bu sebeple bellek ihtiyacı girdi boyutu ile doğrusal orantılıdır ve $O(N)$ ekstra alan tüketir. Graf mesafelerini tutan iki boyutlu matris girdinin kendisi olduğu için ek karmaşıklığa dahil edilmez.
 
