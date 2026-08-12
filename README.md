# Algorithm Analysis

Bu proje, klasik algoritma ve optimizasyon problemlerinin **farklı çözüm yaklaşımlarıyla** C++ dilinde uygulanmasını ve bu yaklaşımların zaman/alan karmaşıklığı açısından karşılaştırılmasını amaçlamaktadır.

Her problem kendi klasoründe yer alır ve o probleme ait farklı algoritmik yaklaşımlar, alt klasörler halinde ayrı ayrı implemente edilmiştir. Her klasörde:

- `main.cpp` — algoritmanın C++ implementasyonu
- `README.md` — o alt klasöre özel açıklama

bulunur. Ayrıca her problem klasörünün kök dizininde, o probleme ait tüm yaklaşımların **karşılaştırmalı analizini** içeren bir `README.md` dosyası vardır (problem tanımı, zorluk analizi, karmaşıklık karşılaştırma tablosu ve algoritmaların hangi durumlarda tercih edilip edilmeyeceği).

## İçindekiler

| Problem | Açıklama | Yaklaşımlar |
|---|---|---|
| [TSP](./TSP) | Gezgin Satıcı Problemi | [Brute-Force](./TSP/Brute-Force), [Nearest-Neighbor](./TSP/Nearest-Neighbor), [Held-Karp](./TSP/Held-Karp), [Branch-and-Bound](./TSP/Branch-and-Bound) |
| [Knapsack](./Knapsack) | 0/1 Sırt Çantası Problemi | [Recursive](./Knapsack/Recursive), [Memoization](./Knapsack/Memoization), [Bottom-Up-DP](./Knapsack/Bottom-Up-DP), [Branch-and-Bound](./Knapsack/Branch-and-Bound) |
| [Sudoku](./Sudoku) | 9×9 Sudoku Çözücü (CSP) | [Backtracking](./Sudoku/Backtracking), [MRV](./Sudoku/MRV) |
| [N_Queens](./N_Queens) | N-Vezir Problemi | [Backtracking](./N_Queens/Backtracking), [Bitmask](./N_Queens/Bitmask) |
| [GraphColoring](./GraphColoring) | Graf Boyama Problemi | [Greedy](./GraphColoring/Greedy), [DSATUR](./GraphColoring/DSATUR) |
| [VertexCover](./VertexCover) | Köşe Örtüsü Problemi | [Exact](./VertexCover/Exact), [2-Approximation](./VertexCover/2-Approximation) |
| [BinPacking](./BinPacking) | Kutu Paketleme Problemi | [First-Fit](./BinPacking/First-Fit), [Best-Fit](./BinPacking/Best-Fit), [FFD](./BinPacking/FFD) |
| [MaximumFlow](./MaximumFlow) | Maksimum Akış Problemi | [Ford-Fulkerson](./MaximumFlow/Ford-Fulkerson), [Edmonds-Karp](./MaximumFlow/Edmonds-Karp), [Dinic](./MaximumFlow/Dinic) |
| [StringMatching](./StringMatching) | Dizi Eşleştirme Problemi | [Naive](./StringMatching/Naive), [KMP](./StringMatching/KMP), [Rabin-Karp](./StringMatching/Rabin-Karp), [Z-Algorithm](./StringMatching/Z-Algorithm) |
| [ConvexHull](./ConvexHull) | Dış Bükey Zarf Problemi | [Jarvis-March](./ConvexHull/Jarvis-March), [Graham-Scan](./ConvexHull/Graham-Scan) |

Her problem isminin üzerine tıklayarak, o probleme ait yaklaşımların detaylı karşılaştırmalı analizine (zaman/alan karmaşıklığı tablosu ve tercih edilme kriterleri) ulaşabilirsiniz.

## Nasıl Çalıştırılır

Her `main.cpp` dosyası bağımsız olarak derlenip çalıştırılabilir. Örneğin:

```bash
g++ -o main main.cpp
./main
```

> Not: `.exe` dosyaları önceden derlenmiş Windows çıktılarıdır ve `.gitignore` ile takip dışı bırakılmıştır; kaynak kodları kendi ortamınızda yeniden derleyerek çalıştırmanız önerilir.

## Amaç

Bu proje, aynı probleme farklı algoritmik stratejilerle (brute-force, greedy, dinamik programlama, dallan-ve-sınırla, yaklaşım algoritmaları vb.) yaklaşarak:

- Her yaklaşımın **zaman ve alan karmaşıklığını** somut biçimde karşılaştırmayı,
- Kesin (exact) çözümler ile sezgisel/yaklaşık (heuristic/approximation) çözümler arasındaki **ödünleşimleri (trade-off)** göstermeyi,
- Hangi yaklaşımın hangi problem büyüklüğü ve senaryoda tercih edilmesi gerektiğini analiz etmeyi

hedefler.
