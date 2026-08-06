# N Vezir (N-Queens) Problemi

N Vezir Problemi, `N × N` boyutundaki bir satranç tahtasına, birbirini hiçbir şekilde tehdit etmeyecek şekilde `N` adet vezirin yerleştirilmesini amaçlayan klasik bir kombinatoryal problem ve kısıt sağlama (constraint satisfaction) problemidir.

### Önemli iki kural vardır:

1. Hiçbir iki vezir aynı satırda veya aynı sütunda bulunamaz.
2. Hiçbir iki vezir aynı çaprazda (diagonal) bulunamaz.

Amaç, bu şartları sağlayan geçerli bir yerleşimi (veya böyle bir yerleşimin bulunmadığını) tespit etmektir.

### Problemin Zorluğu:

N Vezir'in en önemli zorluğu, olası yerleşim sayısının `N` arttıkça patlarcasına büyümesidir. Her satıra bir vezir konulacağı düşünülse bile, sütun seçimlerinin kombinasyonu `N^N` mertebesindedir. Tüm bu kombinasyonları tek tek üretip en sonda geçerliliğini kontrol etmek (Brute Force), `N` büyüdükçe pratik olarak imkansız hâle gelir.

Bu nedenle, N Vezir probleminde bütün yerleşimleri sonuna kadar üretmek yerine, geçersiz bir duruma düşüldüğü anda o dalı terk edip bir önceki adıma dönen **Backtracking (Geri İzleme)** yaklaşımı kullanılır. Backtracking'in kendisi de, güvenli karelerin nasıl tespit edildiğine bağlı olarak farklı şekillerde uygulanabilir; bu bölümde klasik döngü tabanlı kontrol ile bitwise işlemlere dayanan optimize edilmiş kontrol karşılaştırılmaktadır.

## Algoritmaların Karşılaştırılması

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Güvenlik Kontrolü Maliyeti | Zaman Karmaşıklığı (Worst) | Alan Karmaşıklığı |
|---|---|---:|---:|---:|---:|
| Backtracking (Klasik) | Sütun sütun ilerler, her kareyi `isSafe` ile döngüyle kontrol eder | Evet | `O(N)` | `O(N!)` | `O(N²)` |
| Bitmask ile Backtracking | Satır satır ilerler, güvenli sütunları tek bitwise işlemle bulur | Evet | `O(1)` | `O(N!)` | `O(N²)` |

- **Klasik Backtracking**, her aday kareyi `isSafe` fonksiyonuyla satır ve iki çaprazı tek tek kontrol ederek değerlendirir. Kodlanması ve anlaşılması kolaydır, ancak her kontrol `O(N)` sürede çalışır.
- **Bitmask ile Backtracking**, dolu sütunları ve çaprazları birer tamsayı üzerinde bit maskesi olarak tutarak, bir satırdaki tüm güvenli sütunları tek bir bitwise işlemle (`O(1)`) hesaplar. Gezilen arama ağacı (düğüm sayısı) klasik yöntemle asimptotik olarak aynıdır; fark, düğüm başına harcanan süredir.
- Her iki yöntem de aynı kısıtları uyguladığı için, aynı `N` değeri için **aynı çözümü** bulur; aralarındaki fark doğruluktan değil, performanstan (sabit çarpandan) kaynaklanır.

## Algoritmaların Tercih Edilme Durumları

### Backtracking (Klasik)

**Önceki yöntemlere (Brute Force) göre avantajı nedir?** Bütün `N^N` olasılığı sonuna kadar üretip kontrol etmek yerine, geçersiz bir yerleşime ulaşıldığı anda o dalı terk ederek (**erken budama / pruning**) arama uzayını daraltır. Çalışma mantığı ve kodlanması oldukça basit ve anlaşılırdır.

**Hangi durumda tercih edilir?** Kodun okunabilirliğinin ve öğretici olmasının öncelikli olduğu, `N` değerinin makul (küçük-orta) boyutlarda olduğu ve performansın kritik olmadığı durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** `N` değerinin büyük olduğu, performansın kritik olduğu durumlarda tercih edilmez; her aday kare için ayrı ayrı çalışan `O(N)` maliyetli `isSafe` kontrolü, büyük `N` değerlerinde belirgin bir yavaşlığa yol açar.

### Bitmask ile Backtracking

**Önceki yöntemlere göre avantajı nedir?** Klasik `isSafe` tabanlı kontrolün `O(N)` maliyetini, dolu sütun ve çaprazları bit maskesi olarak tutup tek bir `~(cols | leftDiag | rightDiag) & FULL_BOARD` işlemiyle `O(1)`'e indirir. Aynı arama ağacını gezerken önemli ölçüde daha az işlem yapar.

**Hangi durumda tercih edilir?** `N` değerinin klasik backtracking için bile zorlayıcı olduğu, performansın kritik olduğu ve aynı probleme ait çok sayıda çözümün (veya çözüm sayısının) hızlıca bulunması gereken durumlarda tercih edilir.

**Hangi durumda tercih edilmez?** `N` değerinin, kullanılan tamsayı türünün bit genişliğini (örneğin 32-bit `int` için `N > 32`) aştığı durumlarda; `__builtin_ctz` gibi derleyiciye özgü (GCC/Clang) fonksiyonların desteklenmediği taşınabilir kod gerektiren ortamlarda; ve kodun okunabilirliğinin performanstan daha öncelikli olduğu eğitim amaçlı senaryolarda tercih edilmez.