# 🚀 GitHub 仓库规范化操作指南（零基础）

本文档一步步教你如何将整理好的代码上传到你的仓库：
https://github.com/ykn-cpu/GameoftheAmazons

---

## 你当前仓库的问题

- 4 个文件没有 `.cpp` 扩展名
- 没有 README.md（别人看不到项目说明）
- 没有目录结构，全平铺在根目录
- 没有 .gitignore（编译产物可能被误传）

## 整理后的结构（已为你准备好）

```
GameoftheAmazons/
├── README.md
├── .gitignore
├── src/
│   ├── console/main.cpp      ← 控制台基础版（TA 框架）
│   ├── bot/bot.cpp           ← Botzone AI 最终版（调参后）
│   └── gui/
│       ├── main.cpp           ← GUI 最终版（全鼠标 + 最强 AI）
│       └── v1_basic.cpp       ← GUI 中间备份（简易启发式）
├── docs/
│   ├── requirements.md        ← 作业原始要求
│   └── design.md              ← 设计思路与算法说明
└── assets/screenshots/
```

你的 4 个原始文件对应关系：

| 原文件名 | 新位置 |
|----------|--------|
| `main` | `src/console/main.cpp` |
| `Amazon-AI` | `src/bot/bot.cpp` |
| `Easyx` | `src/gui/v1_basic.cpp` |
| `amazon-GUI` | `src/gui/main.cpp` |

---

## 方法一：GitHub 网页端（最简单，推荐）

### 第 1 步：删除旧文件

1. 打开 https://github.com/ykn-cpu/GameoftheAmazons
2. **逐个进入**这 4 个文件 → 点击右上角 `...` → **Delete file** → 确认：
   - `main`
   - `Amazon-AI`
   - `amazon-GUI`
   - `Easyx`

### 第 2 步：上传新文件

1. 回到仓库主页，点击 **Add file → Upload files**
2. 打开桌面上的 `GameoftheAmazons` 文件夹
3. **把整个文件夹内容拖拽**到浏览器上传区域
4. 在 "Commit message" 输入：`重构：规范化目录结构，添加文档`
5. 点击 **Commit changes**

### 第 3 步：检查

刷新 https://github.com/ykn-cpu/GameoftheAmazons，应该看到 README.md 自动渲染为项目主页，目录结构清晰。

---

## 方法二：Git 命令行（如果你装过 Git）

打开 Git Bash，逐条执行：

```bash
# 克隆仓库
git clone https://github.com/ykn-cpu/GameoftheAmazons.git
cd GameoftheAmazons

# 删除旧文件
git rm main Amazon-AI amazon-GUI Easyx

# 复制新文件（把 GameoftheAmazons 文件夹内容全部粘贴到这里）
# 然后用文件管理器操作即可，粘贴完回 Git Bash：

git add .
git status          # 确认变更
git commit -m "重构：规范化目录结构，添加文档"
git push origin main
```

---

## 完成检查

打开仓库主页确认：
- [ ] README.md 显示为项目主页
- [ ] `src/console/main.cpp` 存在
- [ ] `src/bot/bot.cpp` 存在
- [ ] `src/gui/main.cpp` 和 `src/gui/v1_basic.cpp` 存在
- [ ] `docs/` 下有文档
- [ ] `.gitignore` 存在

全部打勾即完成！

---

> 有问题随时问。
