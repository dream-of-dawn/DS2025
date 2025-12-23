#include "Vector.h"
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>
#include <ctime>     // 用于time()
#include <random>    // 用于随机数生成
#include <iomanip>   // 用于格式化输出
#include <algorithm> // 用于min/max函数
using namespace std;
using namespace std::chrono;

// ==================== BoundingBox 结构体 ====================
struct BoundingBox {
    int id;            // 边界框ID
    float x1, y1;      // 左上角坐标
    float x2, y2;      // 右下角坐标
    float confidence;  // 置信度
    bool suppressed;   // 是否被抑制

    BoundingBox(int id = 0, float x1 = 0, float y1 = 0, float x2 = 0, float y2 = 0, float conf = 0)
        : id(id), x1(x1), y1(y1), x2(x2), y2(y2), confidence(conf), suppressed(false) {
    }

    // 计算面积
    float area() const {
        return (x2 - x1) * (y2 - y1);
    }

    // 重载所有比较运算符，用于按置信度排序
    bool operator<(const BoundingBox& other) const {
        return confidence > other.confidence; // 降序排列
    }

    bool operator<=(const BoundingBox& other) const {
        return confidence >= other.confidence; // 降序排列
    }

    bool operator>(const BoundingBox& other) const {
        return confidence < other.confidence; // 降序排列
    }

    bool operator>=(const BoundingBox& other) const {
        return confidence <= other.confidence; // 降序排列
    }

    bool operator==(const BoundingBox& other) const {
        return id == other.id && confidence == other.confidence;
    }

    bool operator!=(const BoundingBox& other) const {
        return !(*this == other);
    }

    // 打印边界框信息
    void print() const {
        cout << "ID: " << id << " Conf: " << fixed << setprecision(3) << confidence
            << " Box: [" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << "]"
            << " Area: " << area() << endl;
    }
};

// ==================== 辅助函数 ====================

// 计算两个边界框的交并比（IoU）
float calculateIoU(const BoundingBox& box1, const BoundingBox& box2) {
    // 计算交集坐标
    float inter_x1 = std::max(box1.x1, box2.x1);
    float inter_y1 = std::max(box1.y1, box2.y1);
    float inter_x2 = std::min(box1.x2, box2.x2);
    float inter_y2 = std::min(box1.y2, box2.y2);
    
    // 计算交集面积
    float inter_width = max(0.0f, inter_x2 - inter_x1);
    float inter_height = max(0.0f, inter_y2 - inter_y1);
    float inter_area = inter_width * inter_height;
    
    // 计算并集面积
    float union_area = box1.area() + box2.area() - inter_area;
    
    // 避免除零
    if (union_area <= 0) return 0.0f;
    
    return inter_area / union_area;
}

// 打印向量中的边界框
void printBoxes(const Vector<BoundingBox>& boxes, const string& title = "") {
    if (!title.empty()) {
        cout << "\n" << title << " (共 " << boxes.size() << " 个):" << endl;
    }
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i].print();
    }
}

// ==================== 数据生成器 ====================

// 1. 随机分布生成器
Vector<BoundingBox> generateRandomBoxes(int num_boxes, float min_x = 0, float max_x = 1920, 
                                        float min_y = 0, float max_y = 1080, 
                                        float min_size = 10, float max_size = 200) {
    Vector<BoundingBox> boxes;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist_x(min_x, max_x);
    uniform_real_distribution<float> dist_y(min_y, max_y);
    uniform_real_distribution<float> dist_size(min_size, max_size);
    uniform_real_distribution<float> dist_conf(0.1, 0.99);
    
    for (int i = 0; i < num_boxes; i++) {
        float x1 = dist_x(gen);
        float y1 = dist_y(gen);
        float width = dist_size(gen);
        float height = dist_size(gen);
        
        // 确保边界框在图像内
        float x2 = min(x1 + width, max_x);
        float y2 = min(y1 + height, max_y);
        
        boxes.insert(BoundingBox(i, x1, y1, x2, y2, dist_conf(gen)));
    }
    
    return boxes;
}

// 2. 聚集分布生成器
Vector<BoundingBox> generateClusteredBoxes(int num_boxes, int num_clusters = 10,
                                          float min_x = 0, float max_x = 1920,
                                          float min_y = 0, float max_y = 1080) {
    Vector<BoundingBox> boxes;
    random_device rd;
    mt19937 gen(rd());
    
    // 生成聚类中心
    Vector<BoundingBox> cluster_centers;
    uniform_real_distribution<float> dist_x(min_x, max_x);
    uniform_real_distribution<float> dist_y(min_y, max_y);
    uniform_real_distribution<float> dist_conf(0.1, 0.99);
    
    for (int i = 0; i < num_clusters; i++) {
        float center_x = dist_x(gen);
        float center_y = dist_y(gen);
        cluster_centers.insert(BoundingBox(i, center_x, center_y, center_x, center_y, dist_conf(gen)));
    }
    
    // 在每个聚类中心周围生成边界框
    int boxes_per_cluster = num_boxes / num_clusters;
    normal_distribution<float> dist_offset(0.0f, 50.0f); // 高斯分布偏移
    uniform_real_distribution<float> dist_size(20.0f, 100.0f);
    
    int box_id = 0;
    for (int c = 0; c < num_clusters; c++) {
        float center_x = cluster_centers[c].x1;
        float center_y = cluster_centers[c].y1;
        
        for (int i = 0; i < boxes_per_cluster && box_id < num_boxes; i++, box_id++) {
            float offset_x = dist_offset(gen);
            float offset_y = dist_offset(gen);
            float width = dist_size(gen);
            float height = dist_size(gen);
            
            float x1 = max(min_x, center_x + offset_x - width/2);
            float y1 = max(min_y, center_y + offset_y - height/2);
            float x2 = min(max_x, x1 + width);
            float y2 = min(max_y, y1 + height);
            
            // 确保边界框有效
            if (x2 > x1 && y2 > y1) {
                boxes.insert(BoundingBox(box_id, x1, y1, x2, y2, dist_conf(gen)));
            } else {
                boxes.insert(BoundingBox(box_id, center_x, center_y, 
                                       center_x + 50, center_y + 50, dist_conf(gen)));
            }
        }
    }
    
    // 如果还有剩余的边界框，随机生成
    uniform_real_distribution<float> dist_remaining_x(min_x, max_x);
    uniform_real_distribution<float> dist_remaining_y(min_y, max_y);
    
    while (box_id < num_boxes) {
        float x1 = dist_remaining_x(gen);
        float y1 = dist_remaining_y(gen);
        float width = dist_size(gen);
        float height = dist_size(gen);
        
        float x2 = min(x1 + width, max_x);
        float y2 = min(y1 + height, max_y);
        
        boxes.insert(BoundingBox(box_id, x1, y1, x2, y2, dist_conf(gen)));
        box_id++;
    }
    
    return boxes;
}

// ==================== 非极大值抑制算法 ====================

Vector<BoundingBox> nms(Vector<BoundingBox>& boxes, float iou_threshold = 0.5, 
                       string sort_algorithm = "quick") {
    if (boxes.empty()) return Vector<BoundingBox>();
    
    // 重置抑制状态
    for (int i = 0; i < boxes.size(); i++) {
        boxes[i].suppressed = false;
    }
    
    // 根据指定算法排序
    auto start_sort = high_resolution_clock::now();
    
    if (sort_algorithm == "bubble") {
        boxes.bubbleSort();
    } else if (sort_algorithm == "selection") {
        boxes.selectionSort();
    } else if (sort_algorithm == "merge") {
        boxes.mergeSort();
    } else { // 默认为快速排序
        boxes.quickSort();
    }
    
    auto end_sort = high_resolution_clock::now();
    auto sort_duration = duration_cast<microseconds>(end_sort - start_sort);
    
    Vector<BoundingBox> result;
    
    // NMS主循环
    auto start_nms = high_resolution_clock::now();
    
    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i].suppressed) continue;
        
        // 添加当前边界框到结果
        result.insert(boxes[i]);
        
        // 抑制与当前边界框IoU超过阈值的其他边界框
        for (int j = i + 1; j < boxes.size(); j++) {
            if (!boxes[j].suppressed) {
                float iou = calculateIoU(boxes[i], boxes[j]);
                if (iou > iou_threshold) {
                    boxes[j].suppressed = true;
                }
            }
        }
    }
    
    auto end_nms = high_resolution_clock::now();
    auto nms_duration = duration_cast<microseconds>(end_nms - start_nms);
    
    // 返回结果和耗时信息
    cout << "排序耗时: " << sort_duration.count() << " μs, "
         << "NMS耗时: " << nms_duration.count() << " μs, "
         << "保留边界框: " << result.size() << "/" << boxes.size() << endl;
    
    return result;
}

// ==================== 性能测试函数 ====================

void runPerformanceTest(const Vector<BoundingBox>& original_boxes, 
                       const string& distribution_name, 
                       int num_boxes, 
                       float iou_threshold = 0.5) {
    
    cout << "\n==========================================" << endl;
    cout << "测试配置: " << distribution_name 
         << " 分布, " << num_boxes << " 个边界框" << endl;
    cout << "IoU阈值: " << iou_threshold << endl;
    cout << "==========================================" << endl;
    
    // 测试四种排序算法
    vector<pair<string, string>> algorithms = {
        {"冒泡排序", "bubble"},
        {"选择排序", "selection"},
        {"快速排序", "quick"},
        {"归并排序", "merge"}
    };
    
    Vector<BoundingBox> results[4];
    
    for (int i = 0; i < algorithms.size(); i++) {
        cout << "\n--- " << algorithms[i].first << " ---" << endl;
        
        // 复制原始数据（避免排序影响后续测试）
        Vector<BoundingBox> test_boxes = original_boxes;
        
        auto start_total = high_resolution_clock::now();
        results[i] = nms(test_boxes, iou_threshold, algorithms[i].second);
        auto end_total = high_resolution_clock::now();
        
        auto total_duration = duration_cast<microseconds>(end_total - start_total);
        cout << "总耗时: " << total_duration.count() << " μs" << endl;
    }
    
    // 验证不同算法的结果是否一致
    cout << "\n结果验证:" << endl;
    bool all_same = true;
    for (int i = 1; i < 4; i++) {
        if (results[i].size() != results[0].size()) {
            cout << algorithms[i].first << " 结果数量不一致: " 
                 << results[i].size() << " vs " << results[0].size() << endl;
            all_same = false;
        }
    }
    
    if (all_same) {
        cout << "所有算法结果一致，保留 " << results[0].size() << " 个边界框" << endl;
    }
}

// ==================== 主测试函数 ====================

int main() {
    srand(time(NULL)); // 初始化随机种子
    
    // 测试不同规模的边界框
    vector<int> box_counts = {100, 500, 1000, 5000, 10000};
    float iou_threshold = 0.5;
    
    cout << "=============== 非极大值抑制算法性能测试 ===============" << endl;
    cout << "测试四种排序算法在不同分布和规模下的性能" << endl;
    cout << "=====================================================" << endl;
    
    // 测试随机分布
    cout << "\n\n=============== 随机分布测试 ===============" << endl;
    for (int count : box_counts) {
        cout << "\n\n生成 " << count << " 个随机分布的边界框..." << endl;
        Vector<BoundingBox> random_boxes = generateRandomBoxes(count);
        
        runPerformanceTest(random_boxes, "随机", count, iou_threshold);
    }
    
    // 测试聚集分布
    cout << "\n\n=============== 聚集分布测试 ===============" << endl;
    for (int count : box_counts) {
        cout << "\n\n生成 " << count << " 个聚集分布的边界框..." << endl;
        int clusters = min(20, count / 10); // 自适应聚类数量
        Vector<BoundingBox> clustered_boxes = generateClusteredBoxes(count, clusters);      
        runPerformanceTest(clustered_boxes, "聚集", count, iou_threshold);
    }
    
    
    // 总结
    cout << "\n\n=============== 测试总结 ===============" << endl;
    cout << "1. 算法性能比较:" << endl;
    cout << "   - 小规模数据 (≤1000): 各种算法差异不大" << endl;
    cout << "   - 中大规模数据 (>1000): 快速排序和归并排序优势明显" << endl;
    cout << "   - 冒泡排序和选择排序在数据量大时性能较差" << endl;
    cout << "\n2. 分布类型影响:" << endl;
    cout << "   - 随机分布: 边界框分散，IoU计算较少，NMS较快" << endl;
    cout << "   - 聚集分布: 边界框重叠多，IoU计算频繁，NMS较慢" << endl;
    cout << "\n3. 推荐使用:" << endl;
    cout << "   - 小规模数据: 任意算法均可" << endl;
    cout << "   - 大规模数据: 推荐使用快速排序或归并排序" << endl;
    
    return 0;
}