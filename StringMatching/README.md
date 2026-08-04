# String Matching (Dizi Eşleştirme) Problemi

String Matching Problemi, verilen bir metin (`text`) içerisinde, daha kısa bir örüntünün (`pattern`) geçtiği tüm konumların (indekslerin) bulunmasını amaçlayan klasik bir metin işleme (text processing) problemidir.

### Önemli iki kural vardır:

1. Örüntü, metin içinde geçtiği her konumda karakter karakter tam olarak eşleşmelidir (kısmi eşleşme kabul edilmez).
2. Metindeki tüm olası konumlar kontrol edilmeli ve örüntünün geçtiği her konum eksiksiz olarak bulunmalıdır.

Amaç, bu şartları sağlayacak şekilde, örüntünün metin içindeki tüm başlangıç indekslerini mümkün olan en kısa sürede ve en az ek bellek kullanarak bulmaktır.

### Problemin Zorluğu:

String Matching'in temel zorluğu, metin (`N`) ve örüntü (`M`) uzunlukları büyüdükçe, her konumda karakter karakter karşılaştırma yapmanın getirdiği maliyetin hızla artmasıdır. En basit yaklaşım olan Naive algoritma, metnin her konumunda örüntünün tamamını karşılaştırdığı için en kötü durumda `O(N × M)` işlem gerektirir; bu da özellikle tekrar eden karakter desenlerine sahip büyük metinlerde (örneğin DNA dizileri, log dosyaları) pratik olmayan bir performansa yol açar.

Bu nedenle, karakter karşılaştırmasının kendisini azaltmak veya gereksiz tekrar karşılaştırmaları önlemek amacıyla farklı stratejiler geliştirilmiştir: karşılaştırmayı sayısal bir özet (hash) üzerinden yapmak (**Rabin-Karp**), örüntünün kendi içindeki tekrar eden desenlerden yararlanarak metin işaretçisini hiç geri almamak (**KMP**) veya örüntü ile metni birleştirip tek bir dizi üzerinden en uzun ortak önek bilgisini kullanmak (**Z Algoritması**).

## Algoritmaların Karşılaştırılması

| Algoritma | Yaklaşım | Doğrusal Zaman Garantisi | Zaman Karmaşıklığı (Worst) | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Naive | Her konumda örüntüyü baştan karakter karakter karşılaştırır | Hayır | `O(N × M)` | `O(1)` |
| Rabin-Karp | Karşılaştırmayı hash değerleri üzerinden yapar, rolling hash ile pencereyi kaydırır | Hayır (hash çakışmasına bağlı) | `O(N × M)` | `O(1)` |
| Z Algoritması | Örüntü + metni birleştirip Z dizisi (en uzun ortak önek) hesaplar | Evet | `O(N + M)` | `O(N + M)` |
| KMP | Örüntünün önek-sonek (`lps`) bilgisini kullanarak metin işaretçisini hiç geri almaz | Evet | `O(N + M)` | `O(M)` |

- **Naive**, herhangi bir ön işleme gerektirmeyen, çok basit ve doğrudan uygulanabilen bir yöntemdir; ancak tekrar eden karakter desenlerinde performansı hızla düşer.
- **Rabin-Karp**, karşılaştırmayı hash değerleri üzerinden yaparak ortalama durumda hızlı çalışır; ancak hash çakışmaları (spurious hit) yaşandığında Naive kadar yavaşlayabilir. Özellikle birden fazla örüntünün aynı anda arandığı durumlarda avantajlıdır.
- **Z Algoritması**, örüntü ile metni birleştirip tek bir Z dizisi üzerinden çalışarak kesin `O(N + M)` performans garantisi verir; ancak birleştirilmiş dizi nedeniyle `O(N + M)` ek bellek kullanır.
- **KMP**, Z algoritması gibi kesin doğrusal zaman garantisi verir, ancak yalnızca örüntü uzunluğunda (`O(M)`) ek bellek kullanarak bunu daha az bellekle başarır; bu nedenle genellikle pratikte en tercih edilen kesin çözüm yöntemidir.

## Algoritmaların Tercih Edilme Durumları

### Naive

**Önceki yöntemlere (kesin çözümlere) göre avantajı nedir?** Herhangi bir ön işleme veya ek veri yapısı gerektirmeden, doğrudan metin ve örüntüyü karşılaştırarak çok basit ve kolay anlaşılır bir çözüm sunar. Çalışma mantığı ve kodlanması oldukça basittir.

**Hangi durumda tercih edilir?** Metin ve örüntü boyutlarının küçük olduğu, ön işleme maliyetinin gereksiz olduğu ve örüntünün metin içinde yalnızca bir kez veya nadiren arandığı durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Metin ve örüntü boyutlarının büyük olduğu, özellikle tekrar eden karakter desenlerine sahip metinlerde tercih edilmez. En kötü durumda her konumda örüntünün tamamı karşılaştırıldığı için performans ciddi şekilde düşer.

### Rabin-Karp

**Önceki yöntemlere göre avantajı nedir?** Naive algoritmasına kıyasla, karşılaştırmayı önce ucuz bir hash kontrolü üzerinden yapar; karakter karşılaştırması yalnızca hash değerleri eşleştiğinde gerçekleştirilir. Rolling hash sayesinde bir sonraki pencerenin hash değeri `O(1)` sürede hesaplanır.

**Hangi durumda tercih edilir?** Birden fazla örüntünün aynı metin içinde aynı anda arandığı (örneğin intihal/plagiarism tespiti gibi) ve iyi seçilmiş bir asal sayı (`q`) ile hash çakışmalarının az olduğu durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** `q` asal sayısının kötü seçildiği veya metnin hash çakışmalarına çok müsait olduğu (tek tip karakterlerden oluşan) durumlarda tercih edilmez. Bu durumda algoritma Naive kadar yavaşlayabilir (`O(N × M)`).

### Z Algoritması

**Önceki yöntemlere göre avantajı nedir?** Naive'in tekrar karşılaştırma sorununu ve Rabin-Karp'ın hash çakışması riskini ortadan kaldırarak, en kötü durumda dahi kesin olarak `O(N + M)` performans garantisi verir.

**Hangi durumda tercih edilir?** Kesin olarak doğrusal zamanlı bir çözüme ihtiyaç duyulan, metin ve örüntünün büyük olduğu ve performans garantisinin (worst-case dahil) kritik olduğu durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** Bellek kullanımının kısıtlı olduğu ortamlarda tercih edilmez; çünkü örüntü ile metnin birleştirilmesi (`pattern + '$' + text`) `O(N + M)` ek bellek gerektirir.

### KMP (Knuth-Morris-Pratt)

**Önceki yöntemlere göre avantajı nedir?** Z algoritması gibi kesin `O(N + M)` performans garantisi verir, ancak bunu ek bir birleştirilmiş dizi oluşturmadan, yalnızca örüntü uzunluğunda (`O(M)`) bir `lps` dizisiyle başarır. Metin işaretçisi hiçbir zaman geri gitmez.

**Hangi durumda tercih edilir?** Kesin doğrusal zaman garantisinin gerektiği, aynı zamanda bellek kullanımının da önemli olduğu durumlarda; metnin akış (stream) hâlinde işlendiği, metin işaretçisinin geri gidemediği senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Örüntünün çok kısa olduğu veya kendi içinde hiç tekrar eden desen barındırmadığı (`lps` dizisinin tamamen sıfırlardan oluştuğu) durumlarda; bu gibi durumlarda ön işleme adımı ekstra bir avantaj sağlamaz ve Naive algoritma yeterli olabilir.s