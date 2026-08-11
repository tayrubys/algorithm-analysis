#include <bits/stdc++.h>
using namespace std;

//noktanin x ve y koordinatlarini tutan yapi
struct Point {
    double x, y;
    //iki noktanin esit olup olmadigini kontrol eder
    bool operator==(const Point& t) const {
        return x == t.x && y == t.y;
    }
};

// (a, b, c)yonunu bulur -1:saat yonu,1:saat yonunun tersi,0:ayni dogru uzerinde
int orientation(Point a, Point b, Point c) {
    double v = a.x * (b.y - c.y) +
               b.x * (c.y - a.y) +
               c.x * (a.y - b.y);
    if (v < 0) return -1;
    if (v > 0) return +1;
    return 0;
}

//iki nokta arasindaki uzakligin karesini hesaplar
double distSq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) +
           (a.y - b.y) * (a.y - b.y);
}

//verilen 2 boyutlu nokta kumesinin convex hull bulur
vector<vector<int>> findConvexHull(vector<vector<int>> points) {
    int n = points.size();//nokta sayisini tutar

    //en az 3 nokta olmadan convex hull olusturulamaz
    if (n < 3) return {{-1}};

    //2 boyutlu vektordeki noktalari Point yapisina donusturur
    vector<Point> a;

    for (auto& p : points) {
        a.push_back({(double)p[0], (double)p[1]});
    }

    //en kucuk y degerine sahip noktayı bulur,y degerleri esitse x degeri kucuk olan secilir
    Point p0 = *min_element(a.begin(), a.end(),
        [](Point a, Point b) {
            return make_pair(a.y, a.x) < make_pair(b.y, b.x);
        });

    //noktalari p0 a gore polar acilarina gore siralar
    sort(a.begin(), a.end(),
        [&p0](const Point& a, const Point& b) {
            int o = orientation(p0, a, b);
            //noktalar ayni dogru uzerindeyse p0 a daha yakin olan once gelir
            if (o == 0) {
                return distSq(p0, a) < distSq(p0, b);
            }
            //diger durumlarda noktalari saat yonunun tersine gore siralar
            return o < 0;
        });

    //convex hull uzerinde bulunan noktalari tutar
    vector<Point> st;

    //siralanmis butun noktalari sirayla isler
    for (int i = 0; i < (int)a.size(); ++i) {

        //son iki nokta ile mevcut nokta saat yönünde veya aynı doğru üzerindeyse ortadaki noktayı cikar
        while (st.size() > 1 &&
               orientation(st[st.size() - 2], st.back(), a[i]) >= 0) {
            st.pop_back();
        }

        //mevcut noktayi convex hulla ekler
        st.push_back(a[i]);
    }

    //sonucta 3 ten az nokta kaldiysa convex hull olusturulamaz
    if (st.size() < 3) return {{-1}};

    //sonuclari tekrar vector<vector<int>> formatina donusturur
    vector<vector<int>> result;

    for (auto& p : st) {
        result.push_back({(int)p.x, (int)p.y});
    }
    return result;
}

int main() {
    //2 boyutlu nokta kumesi
    vector<vector<int>> points = {
        {0, 0}, {1, -4}, {-1, -5}, {-5, -3}, {-3, -1},
        {-1, -3}, {-2, -2}, {-1, -1}, {-2, -1}, {-1, 1}
    };
    //convex hull hesaplar
    vector<vector<int>> hull = findConvexHull(points);
    //sonuc -1 ise hata durumunu yazdirir
    if (hull.size() == 1 && hull[0].size() == 1) {
        cout << hull[0][0] << " ";
    }
    else {
        for (auto& point : hull) {
            cout << point[0] << ", " << point[1] << "\n";
        }
    }
    return 0;
}