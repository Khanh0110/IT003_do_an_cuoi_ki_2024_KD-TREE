#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <limits>
#include <cmath>

using namespace std;

/**
 * Struct đại diện cho một điểm trong không gian n-chiều.
 * 
 * Point chứa một vector các tọa độ.
 */
struct Point {
    vector<double> toado;
    Point(initializer_list<double> init) : toado(init) {}
    Point() = default;
};

/**
 * Lớp đại diện cho một node trong kd-tree.
 * 
 * KDNode chứa một Point và hai con trỏ unique_ptr trỏ đến các node con trái và phải.
 */
class KDNode {
public:
    Point point;
    unique_ptr<KDNode> lt;
    unique_ptr<KDNode> rt;

    //Nhận một Point và khởi tạo các con trỏ trái và phải là nullptr.
    KDNode(Point p) : point(p), lt(nullptr), rt(nullptr) {}
};

/**
 * Lớp quản lý kd-tree.
 * 
 * KDTree xây dựng và quản lý kd-tree từ một vector các điểm.
 */
class KDTree {
public:
    /**
     * Constructor cho lớp KDTree.
     * 
     * Nhận một vector các điểm và xây dựng kd-tree từ các điểm này.
     */
    KDTree(const vector<Point>& points) {
        root = xdc(points, 0);    // xdc: xây dựng cây
    }

    /**
     * In kd-tree ra màn hình.
     */
    void output() {
        optree(root.get(), 0);
    }

    /**
     * Tìm điểm gần nhất với một điểm cho trước.
     * 
     * Nhận một điểm và trả về điểm gần nhất trong kd-tree.
     * TKGN : tìm kiếm gần nhất.
     */
    Point TKGN(const Point& muctieu) {
        return gannhat(root.get(), muctieu, 0, root->point, numeric_limits<double>::max());
    }

private:
    unique_ptr<KDNode> root;

    /**
     * Xây dựng kd-tree từ vector các điểm.
     * 
     * Hàm đệ quy này nhận một vector các điểm và độ sâu hiện tại của cây.
     * Trả về con trỏ unique_ptr trỏ đến root của cây con được xây dựng.
     */
    unique_ptr<KDNode> xdc(vector<Point> points, int dosau) {
        if (points.empty()) return nullptr;

        size_t k = points[0].toado.size();
        size_t Truc = dosau % k;

        sort(points.begin(), points.end(), [Truc](const Point& a, const Point& b) {
            return a.toado[Truc] < b.toado[Truc];
        });

        size_t tt = points.size() / 2;
        unique_ptr<KDNode> node = make_unique<KDNode>(points[tt]);

        vector<Point> ltPoints(points.begin(), points.begin() + tt);
        vector<Point> rtPoints(points.begin() + tt + 1, points.end());

        node->lt = xdc(ltPoints, dosau + 1);
        node->rt = xdc(rtPoints, dosau + 1);

        return node;
    }

    /**
     * Trợ giúp in kd-tree ra màn hình.
     * 
     * Hàm đệ quy này nhận một con trỏ trỏ đến node hiện tại và độ sâu hiện tại của cây.
     */
    void optree(KDNode* node, int dosau) {
        if (!node) return;
        for (int i = 0; i < dosau; ++i) cout << "  ";
        cout << "(";
        for (size_t i = 0; i < node->point.toado.size(); ++i) {
            cout << node->point.toado[i];
            if (i < node->point.toado.size() - 1) cout << ", ";
        }
        cout << ")" << endl;
        optree(node->lt.get(), dosau + 1);
        optree(node->rt.get(), dosau + 1);
    }

    /**
     * Hàm trợ giúp đệ quy tìm điểm gần nhất.
     * 
     * Nhận một con trỏ trỏ đến node hiện tại, điểm mục tiêu, độ sâu hiện tại,
     * điểm gần nhất hiện tại và khoảng cách gần nhất hiện tại.
     * Trả về điểm gần nhất trong kd-tree.
     */
    Point gannhat(KDNode* node, const Point& muctieu, int dosau, Point best, double bestDist) {
        if (!node) return best;

        double dist = KC(node->point, muctieu);
        if (dist < bestDist) {
            bestDist = dist;
            best = node->point;
        }

        size_t k = muctieu.toado.size();
        size_t Truc = dosau % k;
        double diff = muctieu.toado[Truc] - node->point.toado[Truc];

        KDNode* nearNode = (diff < 0) ? node->lt.get() : node->rt.get();
        KDNode* farNode = (diff < 0) ? node->rt.get() : node->lt.get();

        best = gannhat(nearNode, muctieu, dosau + 1, best, bestDist);
        bestDist = KC(best, muctieu);

        if (abs(diff) < bestDist) {
            best = gannhat(farNode, muctieu, dosau + 1, best, bestDist);
        }

        return best;
    }

    /**
     * Tính khoảng cách Euclidean giữa hai điểm.
     */
    double KC(const Point& a, const Point& b) {
        double dist = 0;
        for (size_t i = 0; i < a.toado.size(); ++i) {
            dist += (a.toado[i] - b.toado[i]) * (a.toado[i] - b.toado[i]);
        }
        return sqrt(dist);
    }
};

int main() {
    // Khởi tạo một vector các điểm
    vector<Point> points;
    int n;
    cin >> n;
    points.resize(n); 
    for (int i = 0; i < n; i++) {
        double x, y;
        cin >> x >> y;
        points[i] = {x, y};
    }
    
    // Xây dựng kd-tree từ các điểm
    KDTree tree(points);
    
    // Tìm điểm gần nhất với một điểm mục tiêu
    double a,b ; 
    cin >> a >>b ;
    Point muctieu = {a, b};
    Point ketqua = tree.TKGN(muctieu);
    cout << "Diem gan nhat voi diem (" << muctieu.toado[0] << ", " << muctieu.toado[1] << ") la ("
         << ketqua.toado[0] << ", " << ketqua.toado[1] << ")" << endl;

    return 0;
}
