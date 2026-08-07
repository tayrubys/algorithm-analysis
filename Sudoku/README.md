# Sudoku Problemi

Sudoku Problemi, `9 × 9` boyutundaki bir tahtada bulunan boş hücrelere, satır/sütun/`3 × 3` kutu kısıtlarını ihlal etmeyecek şekilde `1`'den `9`'a kadar sayılar yerleştirilmesini amaçlayan klasik bir kısıt sağlama (constraint satisfaction) problemidir.

### Önemli iki kural vardır:

1. Aynı sayı; hiçbir satırda, hiçbir sütunda ve hiçbir `3 × 3` kutuda birden fazla bulunamaz.
2. Bütün boş hücreler mutlaka `1`-`9` arasında geçerli bir sayı ile doldurulmalıdır.

Amaç, bu şartları sağlayan bir yerleştirmeyi (varsa) bulmak; bulunamıyorsa çözümün olmadığını kesin olarak ortaya koymaktır.

### Problemin Zorluğu:
Sudoku'nun en önemli zorluğu, arama uzayının teorik olarak `9^(boş hücre sayısı)` seviyesinde devasa büyüklüğe ulaşmasıdır. Boş hücre sayısı arttıkça "hangi hücreye hangi sayıyı yerleştirsem geçerli kalır?" sorusunun olasılıkları katlanarak artar.

Tüm sayı kombinasyonlarını sonuna kadar üretip en sonda geçerliliğini kontrol etmek (Brute Force), zorlayıcı bulmacalarda pratik olarak imkansızdır. Bu nedenle Sudoku çözümünde, geçersiz bir duruma düşüldüğü anda o dalı terk edip bir önceki adıma dönen **Backtracking (Geri İzleme)** yaklaşımı ve bunu hızlandıran **MRV (Minimum Remaining Values)** sezgiseli kullanılır.

## Algoritmaların Karşılaştırılması:

| Algoritma | Yaklaşım | Kesin Sonuç Garantisi | Zaman Karmaşıklığı (En Kötü) | Alan Karmaşıklığı |
|---|---|---:|---:|---:|
| Backtracking (Klasik) | Hücreleri sabit sırayla (soldan sağa, yukarıdan aşağıya) dolaşır, `isSafe` ile denenen sayıyı doğrular | Evet | `O(9^(M))` | `O(N²)` |
| Backtracking + MRV | Her adımda en az adaya sahip (en kısıtlı) boş hücreyi önce seçer, `isSafe` ile denenen sayıyı doğrular | Evet | `O(9^(M))` | `O(N²)` |

- **Klasik Backtracking**, hücreleri her zaman aynı sabit sırayla ele alan basit ve anlaşılır bir yöntemdir; ancak hangi hücrenin daha "zor" (az adaylı) olduğunu önceden değerlendirmediği için, geçersiz dallara bazen gereğinden derin inebilir.
- **MRV ile Backtracking**, her adımda tüm boş hücreleri tarayarak en az geçerli adaya sahip olanı önce doldurur; "önce en zor kararı ver" (**fail-first**) mantığıyla çıkmaz sokakları çok daha erken fark eder ve gereksiz derin aramaları önler. Ancak her adımda ekstra tarama yaptığı için düğüm başına maliyeti daha yüksektir.

**Alan karmaşıklığı notu:** `O(N²)` ifadesi Sudoku tahtasının kendisi de dahil olmak üzere kullanılan toplam alanı belirtir. `N × N` boyutundaki tahta bellekte tutulduğu için `N²` alan gerektirir. Algoritmanın kullandığı ek yardımcı değişkenler ise `O(1)` ek alan kullanır.

## Algoritmaların Tercih Edilme Durumları

### Backtracking (Klasik)

**Önceki yöntemlere (Brute Force) göre avantajı nedir?** Bütün olası tahta durumlarını sonuna kadar üretip en sonda kontrol etmek yerine, bir sayı yerleştirildiği anda geçersizlik tespit edilirse o dalı terk ederek (**pruning**) arama uzayını daraltır. Çalışma mantığı ve kodlanması oldukça basittir.

**Hangi durumda tercih edilir?** Bulmacanın başlangıçta yeterince dolu (ipucu sayısı yüksek) olduğu, dolayısıyla arama uzayının pratikte hızlı daraldığı, standart `9 × 9` gibi makul boyutlu bulmacalarda tercih edilir. Basitlik ve kodun anlaşılırlığının öncelikli olduğu eğitim amaçlı senaryolarda da uygundur.

**Hangi durumda tercih edilmez?** Bulmacanın neredeyse tamamen boş olduğu veya çok az ipucu içerdiği, dolayısıyla arama ağacının çok büyük olduğu zorlayıcı durumlarda tercih edilmez; bu gibi durumlarda saf backtracking yavaş kalabilir.

### Backtracking + MRV

**Önceki yönteme göre avantajı nedir?** Klasik backtracking'in sabit hücre sırasına kıyasla, her adımda en az seçeneği olan hücreyi önce çözerek hatalı dalların çok daha erken tespit edilmesini sağlar. Bir hücrede hiç geçerli aday kalmadıysa (`minChoices == 0`), bu durumu çok daha erken fark ederek gereksiz derin aramaları önler.

**Hangi durumda tercih edilir?** Bulmacanın zor olduğu, çok sayıda boş hücre ve az ipucu içerdiği durumlarda tercih edilir; bu gibi durumlarda MRV'nin erken budama etkisi en çok fark yaratır. Kesin çözümün garanti olarak bulunması gerektiği ancak klasik backtracking'in pratikte çok yavaş kaldığı durumlarda idealdir.

**Hangi durumda tercih edilmez?** Bulmacanın zaten çok kolay olduğu (çoğu hücrenin dolu olduğu) durumlarda tercih edilmez; her adımda tüm boş hücreleri tarayıp aday sayılarını hesaplamanın getirdiği ek yük, klasik backtracking'e kıyasla belirgin bir avantaj sağlamayabilir. Basitlik ve kodun anlaşılırlığının öncelikli olduğu eğitim amaçlı senaryolarda da klasik backtracking daha kolay takip edilebilir.