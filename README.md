# Parallel Programming and System Syllabus
## 核心開發工具鏈 (預計)
* **編譯器：** `gcc`, `g++`, `nvcc`
* **性能分析：** `gprof`, `perf`, `Valgrind`, `NVIDIA Nsight Systems (nsys)`
* **模擬器：** `QEMU` (針對 RTOS 與硬體行為), `GDB`
* **建構工具：** `Make`, `CMake`

## github
https://github.com/jimmy01081122/Parallel-Programming-and-System

## 專案目標 

* **單機多執行緒 (Pthreads/OpenMP)** 跨越到多機通信 (MPI)。
* **掌握硬體架構**：理解 CPU、GPU 與記憶體層次結構。
* **異構偕同** : CPU-GPU
* **OS** : 部分OS端與硬體溝通
* **工具練習**：從多執行緒（Pthreads/OpenMP）到異構計算（CUDA）。


## 共享記憶體並行與 OS 核心交互 (Pthreads 篇) 
* **執行緒生命週期與核心成本**
    * `pthread_create/join` 的系統呼叫開銷量化 ($O(1)$ vs $O(clone)$)。
    * User-level vs Kernel-level threads 映射模型。
* **同步原語的硬體博弈**
    * Mutex (Futex) vs. Spinlock (Busy-wait) vs. Atomic (CAS)。
    * 高競爭下的 Context Switch 成本與 `perf` 數據分析。
* **快取一致性與隱形爭奪 (False Sharing)**
    * Cache Line (64 Bytes) 與 MESI 協定原理。
    * 記憶體對齊 (Alignment) 與填充 (Padding) 優化技術。
* **執行緒親和性與異構架構 (Thread Affinity)**
    * CPU 綁定技術與 `cpu_set_t` 控制。
    * 觀察 P-cores (效能核) 與 E-cores (節能核) 的運算吞吐量差異。
* **整合專案：多核心矩陣特徵提取器**

LAB 1 : https://hackmd.io/@1p0_VOUHTXGcHlBU9A0OEA/SJg8tw5qZl
LAB 2 : https://hackmd.io/vtLtvyLeRJaHAQaWWVUUHQ
LAB 3 : https://hackmd.io/@1p0_VOUHTXGcHlBU9A0OEA/SywA3pXiWe
Project : https://hackmd.io/x3HQckQIQ4G5tklXXDuNwA
## 指令級平行與編譯器自動化 (OpenMP) 
* **高階抽象與 Fork-Join 模型**
    * `#pragma omp` 系列指令的底層編譯轉換。
    * 數據共享屬性：`private`, `shared`, `reduction` 的作用域研究。
* **負載平衡與動態排程 (Scheduling)**
    * 比較 `static`, `dynamic`, `guided` 排程在非均勻任務下的表現。
    * 分析排程開銷 (Overhead) 與執行效率的平衡點。
* **任務並行 (Task Parallelism)**
    * OpenMP Task 語法與遞迴演算法（如 Quicksort）的平行化。
    * 解決不規則資料結構（樹、圖）的並行問題。

LAB 4 : https://hackmd.io/@1p0_VOUHTXGcHlBU9A0OEA/ryUEZv55bx


## CPU 效能調優與演算法設計
* **記憶體牆與快取優化 (Cache Optimization)**
    * Loop Nest Optimization: 矩陣乘法的分塊 (Tiling/Blocking) 策略。
    * Data Layout: 從 Array of Structures (AoS) 轉向 Structure of Arrays (SoA)。
* **效能分析模型 (Roofline Model)**
    * 量化分析：程式是 Compute-bound (受限於算力) 還是 Memory-bound (受限於頻寬)？
    * 使用 `perf` 與 `Intel VTune` (或同等工具) 進行瓶頸定位。

LAB 5 : https://hackmd.io/@1p0_VOUHTXGcHlBU9A0OEA/HkMJabKi-l

## 從 C 到 CUDA — 處理器核心深度操作
**目標：** 理解 SIMT 架構，掌握 Threads, Warps, Blocks, Grids 的硬體映射。

* **核心課題：**
    * **GPU 架構：** 理解 Streaming Multiprocessors (SM) 與隱藏延遲 (Latency Hiding)。
    * **記憶體層級：** Global Memory Coalescing, Shared Memory 銀行衝突 (Bank Conflict)。
    * **數據測量：** 使用 `nsys profile` 查看 Kernel 執行時間與記憶體頻寬利用率。
* **學習資源：**
    * **官方文件：** [NVIDIA CUDA C Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html)。
    * **課程：** UIUC ECE408 / CS483 (Applied Parallel Programming)。
* **實驗任務：** 
    1. 將傳統 C 語言的 Image Blur 轉寫為 CUDA 版本。
    2. 優化 Shared Memory 使用，比較優化前後的運算數據。

## 異構計算與 GPU 加速 (CUDA)
* **GPU 架構與 SIMT 執行模型**
    * Streaming Multiprocessors (SM) 與 Warp Scheduling。
    * CUDA 編程基礎：Kernels, Blocks, Threads 索引映射。
* **GPU 記憶體層次與頻寬優化**
    * Global Memory 合併存取 (Coalescing)。
    * Shared Memory (L1 快取) 手動管理與 Bank Conflict 避讓。
* **異構系統協作 (Async Streams)**
    * CPU-GPU 異步傳輸與計算重疊 (Overlapping)。
當單機效能不足時，需要跨節點（MPI）或利用顯示卡（CUDA/OpenCL）。
* **MPI (Message Passing Interface)**：
    * 學習節點間的通訊：Point-to-point (Send/Recv) 與 Collective (Broadcast, Reduce, Gather)。
* **GPU 程式設計 (CUDA)**：
    * 理解 GPU 的階層式架構：Threads, Blocks, Grids。
    * **記憶體優化**：Global vs. Shared Memory 的存取延遲差異。
* **實作挑戰**：
    * 實作一個簡單的 N-body 模擬（模擬星體引力），比較 CPU 與 GPU 的效能差距。
* **專案建議：** 實作一個「即時影像處理 Pipeline」。
    * **系統層：** 使用多執行緒讀取串流。
    * **計算層：** 使用 CUDA 進行卷積運算。
    * **優化層：** 利用 CUDA Streams 進行計算與資料傳輸的重疊 (Overlapping)。
* **終極指標：** 建立 **Roofline Model** 分析程式是受限於「算力 (Compute Bound)」還是「頻寬 (Memory Bound)」
---

### 專案 (Final Capstone)
* **專案：平行化物理模擬或深度學習算子優化**
    * 要求：結合 OpenMP (CPU 多核) 與 CUDA (GPU 異構)，達成跨硬體的極致效能。


## 額外 進階優化與現代框架
研究如何讓程式「跑得更有質感」。

* **優化技巧**：
    * **Cache Locality**：減少快取失效（Cache Miss）。
    * **False Sharing**：避免多個核心競爭同一個快取行（Cache Line）。
* **現代工具探索**：
    * **Intel TBB** (Threading Building Blocks)：基於任務的平行。
    * **Rust Concurrency**：研究其「所有權」機制如何從編譯階段消除資料競爭。
* **最終專案**：
    * 設計一個平行化的圖像處理濾鏡或簡單的平行化搜尋引擎。

## 額外 處理器與周遭設備交互與 RTOS
**目標：** 在無物理硬體下，透過模擬器學習中斷處理 (Interrupts) 與即時性調度。

* **核心課題：**
    1.  **I/O 行為：** 透過 `mmap` 操作虛擬暫存器，理解 Memory-Mapped I/O (MMIO)。
    2.  **RTOS 實踐：** 在 QEMU 上運行 **FreeRTOS**。
    3.  **調度分析：** 比較 RMS (Rate Monotonic Scheduling) 與 EDF (Earliest Deadline First)。
* **學習資源：**
    * **網站：** [FreeRTOS Official Documentation](https://www.freertos.org/)。
    * **論文：** *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment* (Liu & Layland)。
* **實驗任務：** * 在 QEMU 模擬的 ARM 環境下編寫一個簡單的 UART 驅動程式。
    * 測量 RTOS 的 **中斷延遲 (Interrupt Latency)** 與 **上下文切換 (Context Switch)** 時間。


## 額外 虛擬記憶體與實體映射 (Virtual Memory Subsystem)
* **課程標題：** 分頁機制：從虛擬地址到多級頁表（Page Tables）
* **核心內容：**
    * MMU (Memory Management Unit) 的硬體運作邏輯。
    * Page Fault 的處理流程：Major vs. Minor Page Faults。
    * 記憶體分配器（Allocator）的層級：`brk` 與 `mmap` 的臨界點。
* **研究任務：** * 使用 `mmap` 分配大塊記憶體，並透過 `/proc/[pid]/maps` 與 `smaps` 分析 VMA (Virtual Memory Areas)。
    * **數據測量：** 撰寫隨機存取 vs. 順序存取大陣列的程式，測量 Page Fault 對執行時間的延遲貢獻。
* **資源：** *Understanding the Linux Virtual Memory Manager* (Mel Gorman).
