## Final Project (Phase 1)
Include : **Thread Lifecycle**, **Synchronization**, **False Sharing**, **Affinity**


### 專案題目
多核心異構環境下的矩陣特徵提取器 **(Parallel Matrix Feature Extractor)**

### Spec
給定一個巨大的二維矩陣 $M$ ($16384 \times 16384$，單精度浮點數)，你需要撰寫一個平行程式來計算：
1.  **全局總和 (Global Sum)**：矩陣所有元素的累加。
2.  **行極值 (Row-wise Maxima)**：每一行最大的數值，存入一個陣列。
3.  **效能報告**：量化在不同核心綁定策略下的執行效率。

#### Requirement 1 : Domain Decomposition
* **任務**：將矩陣按「行 (Rows)」平均分配給 $N$ 個執行緒。
* **OS 連結**：利用 **Thread Affinity** 將每個執行緒固定在不同的 CPU 核心上，觀察 P-core 與 E-core 的計算速度落差。

#### Requirement 2 : Reduction Optimization
* **禁忌**：嚴禁多個執行緒直接對一個全域變數加鎖（會產生嚴重的 Contention）。
* **要求**：每個執行緒必須先計算其分配區域的「局部總和 (Local Sum)」，最後再彙整。

#### Requirement 3 : False Sharing Preventing
* **挑戰**：如果你建立一個儲存每個執行緒局部結果的陣列 `float results[NUM_THREADS]`，會觸發 False Sharing。
* **要求**：使用 **Padding** 或 **Thread-local Storage** 確保各核心的 L1 Cache 不會互相打架。

---

### File Structure

```text
parallel-project-01/
├── src/
│   ├── main.c           # 負責初始化矩陣、建立執行緒、匯總結果
│   ├── worker.c         # 核心計算邏輯 (含 Affinity 設定)
│   └── utils.h          # 定義對齊用的結構體
├── scripts/
│   └── run_bench.sh     # 自動測試 1, 2, 4, 8, 16 執行緒的腳本
├── Makefile             # 需包含 -O3 與 -pthread
└── report.md            # 你的研究發現 (核心數據分析)
```

---

### report

 `report.md` 必須包含以下量化分析：

1.  **加速比曲線 (Speedup Curve)**：
    * 計算 $S = T_1 / T_n$。
    * **分析**：當執行緒數量超過實體核心（Physical Cores）進入超執行緒（Hyper-threading）時，加速比是否趨於平緩？為什麼？

2.  **硬體異構性分析 (Heterogeneity)**：
    * 分別測試「只綁定 P-cores」與「只綁定 E-cores」的執行時間。
    * **量化**：計算兩者的 **效能功耗比估算**（若有工具可測）或單純的運算速度倍率。

3.  **Cache Miss 驗證**：
    * 使用 `perf stat -e L1-dcache-load-misses` 比較「有 Padding」與「無 Padding」版本的數據差異。


### 啟動代碼片段 (Data Structure)

為了幫你起步，這裡提供一個符合研究所規範的記憶體對齊結構建議：

```c
#define CACHELINE_SIZE 64

typedef struct {
    double local_sum;
    // Padding 確保每個 struct 佔用整倍數的 Cache Line
    char padding[CACHELINE_SIZE - sizeof(double)]; 
} __attribute__((aligned(CACHELINE_SIZE))) thread_data_t;
```


