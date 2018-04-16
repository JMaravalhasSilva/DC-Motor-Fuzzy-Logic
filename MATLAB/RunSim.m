clc;
clear all;
fuzzy_controller = readfis('Fuzzy_controller2.fis')
sample_time = 0.005;

R=5.39166;
L=4.5371111111*10^-3;
B=0.0050681847;
J=0.0012822507;
Km=0.722435011;
Kb=0.722435011;


dc_tf = tf([Km],[L*J (L*B + R*J) (R*B + Km*Kb)]);

dc_tf_simp = tf([Km],[ (R*J) (R*B + Km*Kb)]);


Motor_sim
