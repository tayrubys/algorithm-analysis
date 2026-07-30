# Branch and Bound Algoritması - TSP

Bu bölümde Gezgin Satıcı Problemini, umut vadetmeyen rotaları erkenden eleyerek kesin sonuca ulaşan Branch and Bound yöntemi açıklanmaktadır.

### Algoritmanın Geliştirilme Amacı

Branch and Bound algoritması, Brute Force yönteminde bulunan bütün rotaları sonuna kadar kontrol etme problemini azaltmak amacıyla geliştirilmiştir.

Algoritma olası rotaları bir arama ağacı şeklinde oluşturur. Ancak bir rotanın mevcut en iyi sonuçtan daha iyi olamayacağı anlaşıldığında bu rota üzerinde ilerlemeyi bırakır. Bu işleme **budama (pruning)** adı verilir.

* **Hangi eksikliği gidermektedir?** Brute Force algoritması, kötü olduğu önceden belli olan rotaları bile sonuna kadar oluşturur. Branch and Bound ise alt sınır değerini kullanarak gereksiz rotaları erkenden eler.
* **Brute Force yöntemine göre avantajı nedir?** Bütün rotaları kontrol etmek yerine yalnızca iyi sonuç verme ihtimali bulunan rotaları araştırır. Buna rağmen en kısa rotayı bulmayı garanti eder.
* **Hangi durumda tercih edilir?** Kesin sonucun gerekli olduğu ve şehir sayısının Brute Force için fazla olmaya başladığı küçük veya orta boyutlu problemlerde tercih edilir.
* **Hangi durumda tercih edilmez?** Şehir sayısının çok fazla olduğu problemlerde öncelik kuyruğunda çok sayıda düğüm tutulabilir. En kötü durumda yeterli budama yapılamadığı için çalışma süresi ve bellek kullanımı çok yüksek olabilir.

### Algoritmanın Çalışma Mantığı

Branch and Bound algoritmasında olası rotalar bir ağaç yapısı şeklinde oluşturulur.

Algoritmanın temel kavramları şunlardır:

* **Branch (Dallanma):** Mevcut şehirden ziyaret edilmemiş şehirlere gidilerek yeni rota seçeneklerinin oluşturulmasıdır.
* **Bound (Sınır):** Bir rotanın ulaşabileceği tahmini en düşük maliyettir.
* **Pruning (Budama):** Alt sınırı mevcut en iyi rota maliyetinden büyük veya eşit olan rotaların elenmesidir.

Algoritmanın çalışma adımları:

1. Başlangıç şehri için kök düğüm oluşturulur.
2. Maliyet matrisi satır ve sütun indirgeme işlemleriyle küçültülür.
3. Matris indirgeme sonucunda kök düğümün alt sınırı hesaplanır.
4. Başlangıç şehrinden gidilebilecek ziyaret edilmemiş şehirler için çocuk düğümler oluşturulur.
5. Her çocuk düğüm için yeni maliyet matrisi ve alt sınır hesaplanır.
6. Alt sınırı en küçük olan düğüm öncelik kuyruğundan alınır.
7. Düğümün alt sınırı mevcut en iyi rota maliyetinden büyük veya eşitse dal budanır.
8. Bütün şehirler ziyaret edildiğinde başlangıç şehrine dönüş maliyeti eklenir.
9. Daha düşük maliyetli bir rota bulunduysa en iyi maliyet ve rota güncellenir.
10. Öncelik kuyruğu boşalana kadar işlem devam eder.

Örneğin şehirler `0, 1, 2, 3` ve başlangıç şehri `0` olsun.

İlk olarak aşağıdaki dallar oluşturulur:

```text
0 → 1
0 → 2
0 → 3
```

Her dal için bir alt sınır hesaplanır. Alt sınırı en küçük olan rota önce araştırılır.

Örneğin daha önce aşağıdaki rota bulunmuş olsun:

```text
0 → 1 → 3 → 2 → 0
```

Bu rotanın maliyeti:

```text
0 → 1 = 10
1 → 3 = 25
3 → 2 = 30
2 → 0 = 15

Toplam maliyet = 80
```

Bu durumda:

```text
bestCost = 80
```

olur.

Başka bir rotanın alt sınırı `85` olarak hesaplanırsa:

```text
85 >= 80
```

olduğu için bu rota `80` değerinden daha iyi olamaz. Bu nedenle rota üzerinde ilerlenmez ve ilgili dal budanır.

Branch and Bound, Brute Force gibi kesin sonucu bulur. Fakat kötü olduğu anlaşılan rotaları tamamlamadan elediği için genellikle daha az işlem yapar.

### Matris İndirgeme İşlemi

Kodda alt sınır hesaplamak amacıyla maliyet matrisi indirgenir.

Örnek bir satır:

```text
INF  10  15  20
```

Bu satırdaki en küçük kullanılabilir değer `10` değeridir.

Satırdaki bütün kullanılabilir değerlerden `10` çıkarılır:

```text
INF  0  5  10
```

Aynı zamanda `10` değeri indirgeme maliyetine eklenir.

Bu işlem:

1. Bütün satırlar için,
2. Daha sonra bütün sütunlar için

uygulanır.

Satır ve sütunlardan çıkarılan minimum değerlerin toplamı, düğümün alt sınır maliyetine eklenir.

Matris indirgeme işleminin amacı, mevcut rotanın tamamlanması için gerekli olabilecek minimum maliyeti tahmin etmektir.

### Child Düğüm Oluşturma

Mevcut şehirden yeni bir şehre gidildiğinde yeni bir çocuk düğüm oluşturulur.

Örneğin:

```text
0 → 1
```

geçişi yapıldığında:

* `0` şehrinden tekrar çıkış yapılmaması için `0` numaralı satır `INF` yapılır.
* `1` şehrine tekrar girilmemesi için `1` numaralı sütun `INF` yapılır.
* Geçiş maliyeti mevcut alt sınıra eklenir.
* Yeni maliyet matrisi tekrar indirgenir.
* Gidilen şehir rotanın sonuna eklenir.

Bu sayede her şehir yalnızca bir kez ziyaret edilir.

### Sözde Kod (Pseudo-Code)

```text
Fonksiyon TSP_BranchAndBound(Graf, BaslangicSehri):

    KökDüğüm oluştur
    KökDüğüm.Matris = Graf
    KökDüğüm.Rota = [BaslangicSehri]
    KökDüğüm.Maliyet = MatrisiIndirge(KökDüğüm.Matris)

    ÖncelikKuyruğu oluştur
    KökDüğümü ÖncelikKuyruğuna ekle

    EnIyiMaliyet = SONSUZ
    EnIyiRota = boş liste

    ÖncelikKuyruğu boş olmadığı sürece:

        Alt sınırı en küçük düğümü kuyruktan çıkar

        Eğer Düğüm.Maliyet >= EnIyiMaliyet ise:
            Bu dalı buda
            Sonraki düğüme geç

        Eğer başlangıç dışındaki bütün şehirler ziyaret edilmişse:

            Başlangıç şehrine dönüş maliyetini ekle

            Eğer ToplamMaliyet < EnIyiMaliyet ise:
                EnIyiMaliyet = ToplamMaliyet
                EnIyiRota = Düğüm.Rota + BaslangicSehri

        Aksi halde:

            Ziyaret edilmemiş her şehir için:

                Yeni çocuk düğüm oluştur
                Kullanılamayan yolları INF yap
                Yeni matrisi indirge
                Çocuk düğümün alt sınırını hesapla

                Eğer Çocuk.Maliyet < EnIyiMaliyet ise:
                    Çocuk düğümü ÖncelikKuyruğuna ekle

    EnIyiMaliyet ve EnIyiRota değerlerini döndür
```

### Kullanılan Veri Yapıları ve STL Özellikleri

### `struct Node`

Arama ağacındaki her rota seçeneğinin bilgilerini tutmak için kullanılır.

Her düğümde şu bilgiler bulunur:

```cpp
vector<vector<int>> matrix;
int cost;
int currentNode;
int level;
vector<int> path;
```

* `matrix`: Düğüm için oluşturulan indirgenmiş maliyet matrisini tutar.
* `cost`: Düğümün alt sınır maliyetini tutar.
* `currentNode`: Şu anda bulunulan şehri tutar.
* `level`: Şimdiye kadar kaç şehir geçişi yapıldığını tutar.
* `path`: Şu ana kadar oluşturulan rotayı tutar.

### `vector<vector<int>>`

Şehirler arasındaki maliyetleri matris şeklinde saklamak için kullanılır.

```cpp
graph[i][j]
```

Bu ifade, `i` numaralı şehirden `j` numaralı şehre gitmenin maliyetini gösterir.

Aynı yapı her düğümün indirgenmiş maliyet matrisini saklamak için de kullanılır.

### `vector<int>`

Şu ana kadar ziyaret edilen şehirlerden oluşan rotayı saklamak için kullanılır.

Örneğin:

```cpp
path = {0, 1, 3};
```

Bu rota şu şekilde gösterilir:

```text
0 → 1 → 3
```

### `priority_queue`

Alt sınırı en küçük olan düğümün önce incelenmesini sağlamak için kullanılır.

```cpp
priority_queue<Node, vector<Node>, CompareNode> pq;
```

Normalde `priority_queue` en büyük değeri önce çıkarır. `CompareNode` karşılaştırma yapısı sayesinde maliyeti en küçük düğüm önce çıkarılır.

### `CompareNode`

Öncelik kuyruğundaki düğümleri alt sınır maliyetlerine göre karşılaştırır.

```cpp
return a.cost > b.cost;
```

Bu karşılaştırma sayesinde daha küçük maliyetli düğüm daha yüksek önceliğe sahip olur.

### `pair<int, vector<int>>`

Fonksiyondan hem en iyi maliyeti hem de en iyi rotayı döndürmek için kullanılır.

```cpp
result.first
```

En kısa rota maliyetini tutar.

```cpp
result.second
```

En kısa rotayı tutar.

### `INF`

Kullanılamayan yolları göstermek için kullanılır.

```cpp
const int INF = INT_MAX;
```

Örneğin bir şehrin kendisine gitmesi yasak olduğu için matrisin köşegen değerleri `INF` olarak tutulur.

```text
0 → 0 = INF
1 → 1 = INF
2 → 2 = INF
3 → 3 = INF
```

---

### Kullanılan Kütüphaneler

* `<iostream>`: Ekrana rota ve maliyet bilgilerini yazdırmak için kullanılır.
* `<vector>`: Maliyet matrislerini ve rota listesini saklamak için kullanılır.
* `<queue>`: `priority_queue` veri yapısını kullanmak için eklenir.
* `<algorithm>`: Matris indirgeme sırasında `min` fonksiyonunu kullanmak için eklenir.
* `<climits>`: `INT_MAX` değerini kullanmak için eklenir.

### Zaman Karmaşıklığı

Branch and Bound algoritmasının çalışma süresi, kaç dalın budanabildiğine bağlıdır.

Alt sınır değerleri başarılı şekilde hesaplanır ve çok sayıda dal budanırsa Brute Force yöntemine göre daha az rota incelenir.

Ancak en kötü durumda hiçbir dal budanamayabilir. Böyle bir durumda bütün olası rota seçenekleri araştırılabilir.

Başlangıç şehri sabit tutulduğunda yaklaşık rota sayısı:

```text
(N - 1)!
```

kadardır.

Bu kodda her düğüm oluşturulurken `N × N` boyutundaki maliyet matrisi kopyalanır ve indirgenir.

Bir matris indirgeme işleminin maliyeti:

```text
O(N²)
```

olur.

Bu nedenle bu uygulamanın en kötü durum zaman karmaşıklığı:

```text
O(N! × N²)
```

şeklinde ifade edilebilir.

Arama uzayının faktöriyel büyümesini vurgulamak için genel olarak:

```text
O(N!)
```

şeklinde de gösterilir.

### Best Case

Alt sınır değerleri sayesinde kötü rotaların büyük bölümü erken aşamada budanır. Bu durumda incelenen düğüm sayısı önemli ölçüde azalır.

Ancak budama miktarı maliyet matrisine bağlı olduğu için sabit bir en iyi durum karmaşıklığı vermek zordur.

### Average Case

Ortalama çalışma süresi:

* Maliyet matrisindeki değerlere,
* Hesaplanan alt sınırların doğruluğuna,
* Bulunan ilk tam rotanın maliyetine,
* Budanan düğüm sayısına

bağlıdır.

Bu nedenle ortalama durum için kesin bir karmaşıklık değeri vermek zordur.

### Worst Case

Hiçbir dalın budanamadığı durumda algoritma bütün rota seçeneklerini araştırabilir.

```text
Worst Case: O(N! × N²) 
```

Matris indirgeme maliyeti göz ardı edilirse arama uzayı genel olarak:

```text
O(N!)
```

şeklinde gösterilir.


### Alan Karmaşıklığı

Her `Node` nesnesinde:

* `N × N` boyutunda bir maliyet matrisi,
* En fazla `N + 1` elemanlı bir rota,
* Maliyet, seviye ve mevcut şehir bilgileri

saklanır.

Tek bir düğümün kullandığı alan:

```text
O(N²)
```

şeklindedir.

Ancak öncelik kuyruğunda aynı anda birden fazla düğüm tutulabilir. En kötü durumda çok fazla rota seçeneği bellekte bekleyebilir.

Bu nedenle en kötü durum alan karmaşıklığı:

```text
O(N! × N²)
```

seviyesine ulaşabilir.

Algoritmanın gerçek bellek kullanımı, öncelik kuyruğunda aynı anda tutulan düğüm sayısına bağlıdır.
