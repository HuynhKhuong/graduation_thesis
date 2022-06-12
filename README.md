# graduation_thesis
The repository containing my graduation thesis' codes and projects (this thesis was done with the collaboration of me and another friend)

# Overview

My graduation thesis is to build a model of a **SCARA** Robotic arm and make use of it in 3D printing. To achieve the target, the thesis focuses on 2 main topics:

**1. Robot arms Kinematics fundamentals**
**2. Integration of 3D printing technology**

**Hardware:** Components built is listed in my thesis report.

# Solutions

Details of solutions are provided in the report of my thesis

## 1. Robot arm kinematics fundamentals

To control Robotic arms, we use the conventional robotic knowledge including:
- **Forward Kinemtaics**: from joints' variables, we calculate the x-y-z coordinate of end-effector.
- **Inverse Kinematics**: from x-y-z coordinate of end-effector, we calculate values of joints' variables.
- **Trajectories planning**: an application of motion control, to prevent massive jerks in robot motions. In this section, we use math algorithms to control robot's velocity and acceleration in relation with time (t) variable.
- **Path planning**: an application of forming path shape in (x-y-z) coordinate. In this section, we map the calculated velocity to a specific path to guide the robot traveling. 

**Code and documentations:** Details of these fundamentals are written in my report. Meanwhile, code and projects are inherited from [a thesis](https://github.com/purelifexx1/grad2.git)  which belongs to my brother who works in same lab

## 2. Integration of 3D printing technology

We implement [FDM](https://all3dp.com/2/fused-deposition-modeling-fdm-3d-printing-simply-explained/) techonology to SCARA robot arm to achieve 3D printing task. To make use of this technology, 2 requirements must be compensated:
- **Temperature**: a high and stable temperature must be ensured to get the best filament's liquid form.
- **Flowrate**: means the pressure created to dispose filament on the printing table. To attain best quality of printing line, this parameter must be modified to meet perfectly with robot speed.

**Code and documentations:** Solutions for those 2 targets are given in my report and the code are provided in the project: [temperature_control](./Temperature_Control/) 
