# Nearest-Neighbor Algoritması — TSP

Bu bölümde Gezgin Satıcı Problemini, bulunulan şehirden ziyaret edilmemiş en yakın şehre giderek çözmeye çalışan Nearest Neighbor yöntemi açıklanmaktadır.

Nearest Neighbor, Brute Force algoritması gibi tüm olası rotaları denemez. Her adımda yalnızca o anda en yakın olan şehri seçtiği için daha hızlı çalışır. Ancak elde edilen rotanın her zaman en kısa rota olacağı garanti edilmez.

## Algoritmanın Geliştirilme Amacı

En Yakın Komşu (Nearest-Neighbor), Açgözlü (Greedy) bir yaklaşımdır. Geliştirilme amacı, kesin ve mükemmel çözümü bulmak değil, devasa veri setlerinde "yeterince iyi" bir çözümü çok hızlı bir şekilde elde etmektir.

- **Hangi eksikliği gidermektedir?** Brute Force gibi algoritmaların faktöriyel zaman karmaşıklığı nedeniyle büyük veri setlerinde kilitlenmesi problemini çözer.
- **Önceki yöntemlere (Brute Force) göre avantajı nedir?** İnanılmaz derecede hızlıdır. Şehir sayısı on binlere çıksa bile saniyeler içinde makul bir rota hesaplayabilir.
- **Hangi durumda tercih edilir?** Hızın mükemmellikten daha önemli olduğu durumlarda, çok büyük şehir ağlarında veya dinamik olarak sürekli değişen gerçek zamanlı rota planlamalarında tercih edilir.
- **Hangi durumda tercih edilmez?** Kesinlikle en kısa (optimal) rotanın bulunması gereken, toleransın sıfır olduğu kritik durumlarda tercih edilmez. Çünkü algoritma başlarda kısa mesafeleri seçerken, sonlara doğru elde kalan birbirinden çok uzak şehirlere gitmek zorunda kalarak genel maliyeti artırabilir (Greedy tuzağı).

## Algoritmanın Çalışma Mantığı (Temel Fikir ve Prensip)

Algoritma, gelecekteki adımları veya büyük resmi umursamaz; sadece mevcut durumda kendisine en az maliyetli olan seçeneği değerlendirir.

1. Belirlenen başlangıç şehrinden yola çıkılır ve bu şehir "ziyaret edildi" olarak işaretlenir.
2. Bulunulan şehirden, henüz ziyaret edilmemiş olan tüm şehirlere olan mesafeler kontrol edilir.
3. Aralarındaki en kısa mesafeye sahip olan şehir seçilir, o şehre gidilir ve "ziyaret edildi" olarak işaretlenir.
4. Ziyaret edilmemiş şehir kalmayana kadar bu işlem tekrarlanır.
5. Son ulaşılan şehirden, başlangıç şehrine dönülerek tur tamamlanır.

## Veri Yapıları

| Yapı | Açıklama |
|---|---|
| `vector<vector<int>>` (Adjacency Matrix) | Şehirler arasındaki maliyetleri $O(1)$ sürede okuyabilmek için kullanılmıştır. |
| `vector<bool>` (Visited Array) | Şehirlerin ziyaret edilip edilmediğini takip etmek için kullanılan tek boyutlu mantıksal bir dizidir. |

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon TSP_NearestNeighbor(Graf, BaslangicSehri):
    ZiyaretEdilenler = Tüm şehirleri "False" yap
    SuAnkiSehir = BaslangicSehri
    ZiyaretEdilenler[SuAnkiSehir] = True
    ToplamMaliyet = 0

    Döngü (Tüm şehirler ziyaret edilene kadar):
        MinMesafe = SONSUZ
        SiradakiSehir = -1

        İç Döngü (Her bir komşu şehir için):
            Eğer (Komşu ziyaret edilmediyse VE Graf[SuAnkiSehir][Komsu] < MinMesafe):
                MinMesafe = Graf[SuAnkiSehir][Komsu]
                SiradakiSehir = Komsu

        ToplamMaliyet = ToplamMaliyet + MinMesafe
        SuAnkiSehir = SiradakiSehir
        ZiyaretEdilenler[SuAnkiSehir] = True

    // Başlangıca dön
    ToplamMaliyet = ToplamMaliyet + Graf[SuAnkiSehir][BaslangicSehri]

    Döndür ToplamMaliyet
```

## Zaman Karmaşıklığı (Time Complexity)
Nearest Neighbor algoritmasında başlangıç şehri ziyaret edildikten sonra geriye kalan N - 1 şehir sırayla seçilir.

Her adımda, mevcut şehirden ziyaret edilmemiş en yakın şehri bulmak için bütün şehirler kontrol edilir.

**Dış döngü yaklaşık:**

```
N - 1
```
kez çalışır

İç döngü ise her adımda toplam N şehri kontrol eder.

**Bu nedenle toplam zaman karmaşıklığı:**
```
O((N - 1) × N) = O(N²)
```

şeklinde gösterilir.

Nearest Neighbor algoritması uygun bir şehri erken bulsa bile, bu şehrin gerçekten en yakın şehir olduğunu belirlemek için diğer şehirleri de kontrol etmek zorundadır. Bu nedenle en iyi, ortalama ve en kötü durumda çalışma süresi aynıdır.

### Best Case — O(N²)
### Average Case - O(N²)
### Worst Case   - O(N²)

Brute Force algoritmasının zaman karmaşıklığı O(N!) iken Nearest Neighbor algoritmasının zaman karmaşıklığı O(N²) olduğu için şehir sayısı arttığında çok daha hızlı çalışır. Ancak Nearest Neighbor algoritması her zaman en kısa rotayı bulmayı garanti etmez.

## Alan Karmaşıklığı (Space Complexity)

- **Alan Karmaşıklığı:** $\mathcal{O}(N)$
- **Ek Bellek Kullanımı:** Hangi şehirlerin ziyaret edildiğini hafızada tutmak için $N$ elemanlı bir `visited` (boolean) dizisi oluşturulmuştur. Bu dizi, girdi boyutuna doğrusal olarak bağlı olduğundan $\mathcal{O}(N)$ ekstra alan kaplar. Graf mesafelerini tutan matris, girdinin kendisi olduğu için analize dahil edilmez.