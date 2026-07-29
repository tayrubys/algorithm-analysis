# TSP(Travelling Salesman Problem)
  Gezgin Satıcı Problemi, bir satıcının belirli şehirleri yalnızca bir kez ziyaret edip başlangıç noktasına geri dönecek şekilde tüm şehirleri en kısa yoldan dolaşmasını amaçlayan klasik bir optimizasyon problemidir.
 
  ### Önemli iki kural vardır:

  1. Başlangıç noktası dışında her şehir yalnızca bir kez ziyaret edilmelidir.
  2. Tüm şehirler ziyaret edildikten sonra başlangıç noktasına geri dönülmelidir.
  
 Amaç, bu şartları sağlayan rotalar arasından toplam mesafesi veya maliyeti en düşük olan rotayı bulmaktır.
 
  ### Problemin Zorluğu: 
  TSP’nin en önemli zorluğu, şehir sayısı arttıkça oluşturulabilecek rota sayısının çok hızlı bir şekilde artmasıdır. Başlangıç şehri sabit tutulduğunda, `n` şehir için yaklaşık `(n-1)!` farklı rota oluşmaktadır.
  
  Örneğin 5 şehir için 24 farklı rota bulunurken, 10 şehir için bu sayı 362.880’e çıkmaktadır. Bu nedenle az sayıda şehir olduğunda tüm rotaları tek tek kontrol etmek mümkün olsa da şehir sayısı arttığında bu yöntem çok fazla zaman almaktadır.