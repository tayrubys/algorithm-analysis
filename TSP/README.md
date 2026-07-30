# TSP(Travelling Salesman Problem)
  Gezgin Satıcı Problemi, bir satıcının belirli şehirleri yalnızca bir kez ziyaret edip başlangıç noktasına geri dönecek şekilde tüm şehirleri en kısa yoldan dolaşmasını amaçlayan klasik bir optimizasyon problemidir.
 
  ### Önemli iki kural vardır:

  1. Başlangıç noktası dışında her şehir yalnızca bir kez ziyaret edilmelidir.
  2. Tüm şehirler ziyaret edildikten sonra başlangıç noktasına geri dönülmelidir.
  
 Amaç, bu şartları sağlayan rotalar arasından toplam mesafesi veya maliyeti en düşük olan rotayı bulmaktır.
 
  ### Problemin Zorluğu: 
  TSP’nin en önemli zorluğu, şehir sayısı arttıkça oluşturulabilecek rota sayısının çok hızlı bir şekilde artmasıdır. Başlangıç şehri sabit tutulduğunda, `n` şehir için yaklaşık `(n-1)!` farklı rota oluşmaktadır.
  
  Örneğin 5 şehir için 24 farklı rota bulunurken, 10 şehir için bu sayı 362.880’e çıkmaktadır. Bu nedenle az sayıda şehir olduğunda tüm rotaları tek tek kontrol etmek mümkün olsa da şehir sayısı arttığında bu yöntem çok fazla zaman almaktadır.

  ## Algoritmaların Karşılştırılması:

  
| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Brute Force | Tüm rotaları dener | Evet | `O(n!)` | `O(n)` |
| Nearest Neighbor | Her adımda en yakın şehri seçer | Hayır | `O(n²)` | `O(n)` |
| Held–Karp | Dinamik programlama ve bitmask kullanır | Evet | `O(n² × 2ⁿ)` | `O(n × 2ⁿ)` |
| Branch and Bound | Alt sınır kullanarak bazı rotaları budar | Evet | En kötü durumda `O(n! × n²)` | İncelenen düğüm sayısına bağlıdır |

- Brute Force, bütün rotaları kontrol ettiği için en kısa rotayı kesin olarak bulur ancak şehir sayısı arttığında çok yavaş çalışır.

- Nearest Neighbor oldukça hızlıdır fakat her zaman en kısa rotayı bulmayı garanti etmez.

- Held–Karp, daha önce hesaplanan alt problemlerin sonuçlarını saklayarak Brute Force yöntemine göre daha verimli bir kesin çözüm sunar.

- Branch and Bound ise iyi sonuç vermeyeceği anlaşılan rotaları erkenden eleyerek incelenen rota sayısını azaltmaya çalışır. Ancak en kötü durumda yine çok fazla rota inceleyebilir.

  ## Algoritmaların Tercih Edilme Durumları

### Brute Force

**Önceki yöntemlere göre avantajı nedir?**  
Bütün olası rotaları kontrol ettiği için en kısa rotayı kesin olarak bulur. Ayrıca uygulanması ve anlaşılması kolaydır.

**Hangi durumda tercih edilir?**  
Şehir sayısının az olduğu problemlerde veya diğer algoritmaların sonuçlarını kontrol etmek için tercih edilir.

**Hangi durumda tercih edilmez?**  
Şehir sayısının fazla olduğu problemlerde tercih edilmez. Çünkü rota sayısı faktöriyel olarak artar ve çalışma süresi çok yükselir.

### Nearest Neighbor

**Önceki yöntemlere göre avantajı nedir?**  
Brute Force yöntemine göre çok daha hızlı çalışır ve büyük şehir sayılarında kısa sürede bir rota oluşturabilir.

**Hangi durumda tercih edilir?**  
Kesin sonuçtan çok hızın önemli olduğu ve kısa sürede uygun bir rota bulunmasının yeterli olduğu durumlarda tercih edilir.

**Hangi durumda tercih edilmez?**  
En kısa rotanın kesin olarak bulunması gereken durumlarda tercih edilmez. Çünkü her zaman optimal sonucu garanti etmez.

### Held–Karp

**Önceki yöntemlere göre avantajı nedir?**  
Brute Force yönteminde tekrar hesaplanan alt problemlerin sonuçlarını saklar. Böylece Brute Force yönteminden daha verimli şekilde kesin sonuç üretir.

**Hangi durumda tercih edilir?**  
Şehir sayısının çok büyük olmadığı ve kesin sonucun gerekli olduğu problemlerde tercih edilir.

**Hangi durumda tercih edilmez?**  
Şehir sayısının fazla olduğu durumlarda tercih edilmez. Çünkü dinamik programlama tablosunun zaman ve bellek ihtiyacı üstel olarak artar.

### Branch and Bound

**Önceki yöntemlere göre avantajı nedir?**  
Brute Force yönteminden farklı olarak, mevcut en iyi sonuçtan daha iyi olamayacağı anlaşılan rotaları tamamlamadan eler. Buna rağmen kesin sonucu bulmayı garanti eder.

**Hangi durumda tercih edilir?**  
Kesin sonucun gerekli olduğu ve Brute Force yönteminin yavaş kalmaya başladığı küçük veya orta büyüklükteki problemlerde tercih edilir.

**Hangi durumda tercih edilmez?**  
Şehir sayısının çok fazla olduğu problemlerde tercih edilmez. Yeterli budama yapılamazsa çalışma süresi ve bellek kullanımı çok yüksek olabilir.