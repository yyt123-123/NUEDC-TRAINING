# NVEDC-TRAINING
This is a plan for Electronic Design Competition
```markdown
# NUEDC-TRAIN 项目组 README

## 项目方向融合说明
### 主方向：高精度运动控制系统（控制+传感融合）
- **核心模块**  
  - **运动控制**：PID算法、步进电机/L298N驱动  
  - **传感融合**：JY901陀螺仪姿态解算 + 地磁计方位校准  
  - **硬件架构**：分层式PCB设计

### 副方向：仪器仪表辅助控制
- **核心模块**  
  - **测量系统**：高精度电压/电流/转速测量（ADC）  
  - **数据闭环**：实时采集控制参数并反馈至PID算法  

---

##融合技术栈与分工
###硬件技术栈
| 模块                  | 技术需求                          | 协同应用场景                 |
|-----------------------|-----------------------------------|------------------------------|
| 陀螺仪+地磁计         | 六轴数据融合（卡尔曼滤波）        | 实时反馈运动姿态至控制算法   |
| ADS1256 ADC模块       | 12位高精度采集（SPI通信）         | 监测电机电流/电压波动        |
| 18650电池（带保护板） | 无                                | 电源驱动                     |
---

### 软件技术栈
| 模块               | 技术需求                          | 如何应用                      |
|--------------------|-----------------------------------|-------------------------------|
| STM32 HAL库        | 掌握基本ADC，PWM，陀螺仪等知识     | 同步控制算法与数据采集         |
| PID控制算法        | 自适应参数调整                    | 调用仪器仪表数据作为反馈量     |
| K210视觉           | 图像识别处理                      | 调用现成视觉库                 |
| GitHub            | 分支管理                          | 遵循GitHub官方文档流程          |

---

## 方向结合
```plaintext
[运动控制闭环]
传感器（陀螺仪/地磁计） → 数据融合 → PID控制器 → 电机驱动  
                          ↑                ↓  
仪器仪表（ADC/基准源） ← 参数采集 ← 电流/电压监测
```

---

## 学习计划与里程碑
### 阶段目标
| 阶段         | 时间       | 控制类任务                          | 仪器仪表类任务                   |
|--------------|------------|-------------------------------------|----------------------------------|
| 基础搭建     | 3.16-3.23  | 实现电机基础PID控制                 | 完成ADC模块数据采集             |
| 数据融合     | 3.24-4.6   | 集成陀螺仪姿态反馈                  | 开发电流-转速关系模型           |
| 精度优化     | 4.7-4.20   | 自适应PID调参算法                   | 校准系统误差（温度/噪声补偿）    |

---

## 设备清单与封箱管理
| 设备                | 数量  | 封箱编号 | 融合用途                          |
|---------------------|-------|----------|-----------------------------------|
| JY901陀螺仪         | 2     | BOX1-A   | 运动姿态反馈                      |
| ADS1256模块         | 2     | BOX5-A   | 电机电流监测                      |
| L298N电机驱动       | 2     | BOX1-C   | 控制执行器                        | 
| STM32F103核心板     | 2     | BOX4     | 主控                             |

---

## GitHub协作规范升级
### 仓库结构
```

### 分支策略
| 分支类型   | 命名规则          | 用途                     |
|---------------|-----------------------|----------------------------|
| main       | 受保护分支        | 仅存放稳定版本           |
| dev        | 开发主分支        | 每日合并各feature分支    |
| feature    | feat/模块名       | 单功能开发（如feat/gyro）|
| hotfix     | fix/问题描述       | 紧急bug修复              |
| yyt        | 成员个人分支        | 提交任务完成进度    |
| qzy    | 成员个人分支       | 提交任务完成进度|
| tr     | 成员个人分支       | 提交任务完成进度     |
---


  

### 提交标签
- `[Control]`：PID调参、电机驱动相关提交  
- `[Instrument]`：测量模块代码/校准算法  
- `[Fusion]`：跨方向整合功能（如数据反馈闭环）  

---

### 优先级排序
1. **第一周速通**：  
   - 控制类：实现基础PID  
   - 仪器类：用现成ADC模块读取电压  
2. **第二周深化**：  
   - 控制类：移植PID到STM32（HAL库模板）  
   - 仪器类：串口绘制实时波形（Serialplot）  
3. **第三周融合**：  
   - 将ADC采集数据作为PID反馈量  
---
