# Game of the Amazons（亚马逊棋）

> **计算概论 A 大作业** · 北京大学 · 2025-2026 学年第一学期

亚马逊棋（Game of the Amazons）是一种双人抽象策略棋类游戏。双方各有 4 枚棋子，每回合依次完成**移动棋子**和**射箭放置障碍**两个动作。当一方无法移动时判负。

本仓库包含从基础到最终的完整演化过程——从满足作业基本要求的控制台版，到 Botzone 平台对战的调参 AI，再到全鼠标交互的图形界面版。

---

## 📁 目录结构

```
├── README.md
├── .gitignore
├── src/
│   ├── console/
│   │   └── main.cpp          ← 字符界面基础版
│   ├── bot/
│   │   └── bot.cpp           ← Botzone AI 最终版
│   └── gui/
│       ├── main.cpp           ← GUI 最终版（全鼠标交互 + 最强 AI）
│       └── v1_basic.cpp       ← GUI 中间备份（简易启发式 AI）
├── docs/
│   ├── requirements.md        ← 作业原始要求
│   └── design.md              ← 设计思路与算法说明
└── assets/screenshots/        ← 截图
```

---

## 🗺️ 项目演化历程

四个文件之间不是并列关系，而是逐步迭代的过程：

```
控制台基础版 ──→ Botzone AI 调参 ──→ GUI 最终版
         │                              │
         │              ┌───────────────┘
         └──→ GUI 中间备份
```

| 文件 | 阶段 | 界面 | AI 水平 | 说明 |
|------|------|------|---------|------|
| `src/console/main.cpp` | 入门 | 字符终端 | 机动性贪心 | 满足作业基本要求，框架借用助教（Credit to TA） |
| `src/bot/bot.cpp` | AI 调参 | 标准输入输出 | Minimax + α-β + 迭代加深 + BFS 领地评估 | 为 Botzone 平台对战反复调参后的最终版 |
| `src/gui/v1_basic.cpp` | GUI 探索 | EasyX 木纹棋盘 | 简易启发式 | GUI 开发中间备份，木纹风格棋盘 |
| `src/gui/main.cpp` | 最终交付 | EasyX 渐变阴影 | **最强 AI**（同 Botzone 版） | 全鼠标三步交互 + 菜单系统 + 状态栏 |

---

## 🎮 各版本功能

### 控制台基础版 `src/console/main.cpp`

> 入门阶段，满足作业基本要求。框架来自助教（在此致谢）。

- 字符界面棋盘（用表格线绘制）
- 三种游戏模式：人机 / 人人 / 机机 对战
- 游戏存盘与读盘（二进制 `.sav`）
- AI：遍历所有合法走法，选择 `mobility(我方) - mobility(对方)` 最大的走法

```bash
g++ -o console src/console/main.cpp -std=c++11 && ./console
```

### Botzone AI 最终版 `src/bot/bot.cpp`

> 为 Botzone 平台对战反复调参，尽可能压榨性能。

- 符合 Botzone 平台输入输出协议（坐标需 swap）
- **Minimax 搜索 + Alpha-Beta 剪枝**
- **迭代加深**：深度 1→5，超时自动退出仍能返回当前最优解
- **走法排序**：用快速评估函数预排序走法，大幅提高剪枝效率
- **多维度评估函数**：机动性 + 障碍封锁 + BFS 领地控制（权重 35×）
- 950ms 时限保护

```bash
g++ -o bot src/bot/bot.cpp -std=c++11 -O2 && ./bot < input.txt
```

### GUI 中间备份 `src/gui/v1_basic.cpp`

> 探索 EasyX 图形库过程中的中间存档。保留在此作为开发历程的记录。

- 木纹风格棋盘 + 传统棋子样式
- 鼠标选子 → 选目标 → 选射箭 三步交互
- 右键存盘、ESC 退出
- AI：遍历所有走法，机动性差值贪心（与控制台版同级别）

### GUI 最终版 `src/gui/main.cpp`

> 最终的图形界面交付版本，交互和 AI 都是最强的。

- 渐变阴影棋盘 + 合法位置红/黄高亮 + 悬停绿色光晕
- **全鼠标三步交互**，带步骤提示
- 主菜单 → 选颜色 → 对战循环，流程完整
- 状态栏：存盘按钮 + 退出按钮
- AI：**与 Botzone 最终版同款**（Minimax + α-β + 迭代加深 + 走法排序）
- 需要 Windows + [EasyX](https://easyx.cn/) 图形库

---

## 🧠 AI 算法概要

评估函数由三维加权构成：

`score = mobility_diff × 1 + block_score × 3 + territory_score × 35`

| 维度 | 计算方式 | 作用 |
|------|----------|------|
| **机动性** | 每棋子 × 每可达方向 × 步数 × 5 | 保持活动空间 |
| **障碍封锁** | 障碍紧邻对方棋子 +10 | 限制对手移动 |
| **领地控制** | BFS 计算双方可达区域，取差值 | 争夺棋盘空间 |

搜索算法：**Minimax + Alpha-Beta 剪枝 + 迭代加深 + 走法排序**。

---

## ⚠️ 环境要求

| 版本 | 编译环境 |
|------|----------|
| 控制台版 | C++11 编译器，无额外依赖 |
| Botzone AI | C++11 编译器，无额外依赖 |
| GUI 两版 | Windows + Visual Studio + [EasyX](https://easyx.cn/) 图形库 |

---

## 📄 声明

- 控制台版框架来自助教（Credit to TA），在此致谢。
- 本项目为课程作业，仅供参考学习。

> *「计算概论 A」大作业 · 2025 秋 · 北京大学*
