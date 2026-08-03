# Bin Packing (Kutu Paketleme) Problemi

Kutu Paketleme Problemi, farklı boyutlara (veya ağırlıklara) sahip bir dizi eşyanın (nesnenin), sabit ve aynı kapasiteye sahip minimum sayıda kutuya yerleştirilmesini amaçlayan klasik bir kombinatoryal optimizasyon problemidir.

### Önemli iki kural vardır:

1. Hiçbir kutunun kapasitesi aşılamaz (Bir kutuya sığmayacak kadar eşya konulamaz).
2. Bütün eşyalar mutlaka bir kutuya yerleştirilmelidir.

Amaç, bu şartları sağlayan yerleştirmeler arasından boşlukları en aza indirerek en az sayıda kutuyu kullanan çözümü (optimum yerleşimi) bulmaktır.

### Problemin Zorluğu:
Bin Packing'in en önemli zorluğu, NP-Hard sınıfında bir problem olmasıdır. Eşya sayısı arttıkça "hangi eşyayı hangi kutuya koysam en az boşluk kalır?" sorusunun olasılıkları astronomik seviyelere ulaşır. 

Tüm kombinasyonları tek tek deneyerek (Brute Force) kesin ve mükemmel sonucu aramak, devasa veri setlerinde pratik olarak imkansızdır. Bu nedenle, Bin Packing probleminde mükemmeli aramak yerine "mükemmele en yakın" ve kabul edilebilir sonucu çok hızlı bir şekilde üreten Açgözlü (Greedy) yaklaşımlar (Heuristic algoritmalar) kullanılır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| First Fit | Sığabileceği ilk kutuya yerleştirir | Hayır | `O(N²)` | `O(N)` |
| Best Fit | En az boşluk bırakacak kutuyu seçer | Hayır | `O(N²)` | `O(N)` |
| First Fit Decreasing (FFD) | Büyükten küçüğe sıralar, ilk uygun kutuya yerleştirir | Hayır (Optimuma çok yakın) | `O(N²)` | `O(N)` |

- **First Fit**, çevrim içi (online) çalışabilen çok basit ve hızlı bir yöntemdir ancak boşlukları optimize etmediği için gereksiz kutu israfına yol açabilir.
- **Best Fit**, kutuların kalan kapasitelerini daha dikkatli değerlendirir ve boşlukları en verimli şekilde doldurmaya çalışır. Ancak doğru kutuyu bulmak için fazladan arama yapar.
- **First Fit Decreasing (FFD)**, büyük boyutlu nesnelerin sona kalıp fazladan kutu açtırması riskini sıfıra indirir. Sıralama kullandığı için genelde en az kutuyu harcayan, optimuma en yakın başarılı algoritmadır.

## Algoritmaların Tercih Edilme Durumları

### First Fit

**Önceki yöntemlere (Kesin Çözümlere) göre avantajı nedir?** Bütün olası yerleştirmeleri denemek yerine, her nesneyi sığdığı ilk kutuya yerleştirerek çok daha kısa sürede, hızlı ve pratik bir çözüm üretir. Çalışma mantığı ve kodlanması oldukça basittir.

**Hangi durumda tercih edilir?** Nesne sayısının çok fazla olduğu, hızlı bir sonuca ihtiyaç duyulduğu ve nesnelerin sisteme sırayla anlık olarak geldiği çevrim içi (online) sistemlerde tercih edilir.

**Hangi durumda tercih edilmez?** Minimum kutu sayısının (sıfır firenin) kesin olarak bulunması gereken durumlarda tercih edilmez. Nesnelerin geliş sırasından doğrudan etkilendiği için optimum çözümden uzaklaşabilir.

### Best Fit

**Önceki yöntemlere göre avantajı nedir?** First Fit algoritmasına kıyasla kutuların kalan kapasitelerini daha dikkatli değerlendirir. Nesneyi sığdığı ilk kutuya atmak yerine, en az boşluk bırakacak kutuyu bularak israfı azaltır.

**Hangi durumda tercih edilir?** Kutuların içindeki boş alanların mümkün olduğunca azaltılmasının istendiği ve sistemin hala "çevrim içi" (online) çalışması gereken senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Nesne sayısının çok fazla olduğu ve her nesne için bütün açık kutuları kontrol etmenin getireceği zaman kaybının tolere edilemediği durumlarda tercih edilmez.

### First Fit Decreasing (FFD)

**Önceki yöntemlere göre avantajı nedir?** First Fit ve Best Fit algoritmalarındaki nesne sırasına bağlılık (rastgelelik) problemini ortadan kaldırır. "Önce büyükleri yerleştir, küçükler kalan aralara sığar" mantığıyla kutu israfını en aza indirerek optimuma en yakın sonucu üretir.

**Hangi durumda tercih edilir?** Nesnelerin tamamının boyutlarının baştan bilindiği çevrim dışı (offline) sistemlerde; kargo yüklemesi veya kumaş kesimi gibi minimum kutu (en az fire) kullanımının son derece kritik olduğu endüstriyel senaryolarda tercih edilir.

**Hangi durumda tercih edilmez?** Nesnelerin sisteme bir ağ üzerinden veya banttan sırayla geldiği ve bütün listenin önceden bilinmediği çevrim içi (online) sistemlerde, sıralama (sorting) yapılamayacağı için kullanılamaz.