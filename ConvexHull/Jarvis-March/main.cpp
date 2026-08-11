#include <iostream>
#include <vector>
using namespace std;

//noktanin x ve y koordinatlarini tutan yapi
struct Point {
    int x, y;
};

//uc noktanin donus yonunu bulur
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;//ayni dogru uzerindeler
    return (val > 0) ? 1 : 2;//1: saat yönü, 2: saat yönünün tersi
}

//jarvis march algoritmasini uygular
void jarvisMarch(vector<Point>& points) {
    int n = points.size();

    //en az 3 nokta olmadan convex hull olusturulamaz
    if (n < 3) {
        cout << "Convex hull not possible.\n";
        return;
    }

    //convex hull noktalarini tutacak vektor
    vector<Point> hull;

    //en soldaki noktayi bul
    int l = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[l].x)
            l = i;
    }

    //baslangic noktasi en soldaki nokta
    int p = l;//su an bulundugumuz nokta
    int q;//bir sonraki hull noktasi

    //baslangic noktasina tekrar donene kadar devam et
    do {
        //mevcut noktayi convex hull'a ekle
        hull.push_back(points[p]);
        //baslangicta bir sonraki nokta olarak p + 1'i sec
        q = (p + 1) % n;
        //diger butun noktalari kontrol et
        for (int i = 0; i < n; i++) {
            //daha dis tarafta bir nokta bulunduysa
            if (orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }
        //buldugumuz noktaya gec
        p = q;

    } while (p != l);
    //convex hull noktalarini yazdir
    cout << "Points in Convex Hull:\n";
    for (auto& pt : hull)
        cout << "(" << pt.x << ", " << pt.y << ")\n";
}

int main() {
    vector<Point> points = {
        {0, 0}, {1, 1}, {2, 2}, {2, 0},
        {1, 2}, {0, 2}
    };
    jarvisMarch(points);
    return 0;
}