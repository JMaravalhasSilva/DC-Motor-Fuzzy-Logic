# DC Motor Fuzzy-Logic control using a XMC-4700

A DC motor fuzzy logic speed and position controller implemented in a XMC-4700 using DAVE4.

The hardware used is a Infineon's [XMC-4700 Relax kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc47_relax_v1/), Infineon's [DC motor driver board based on the BTN8982](https://www.infineon.com/cms/en/product/evaluation-boards/dc-motorcontr_btn8982/) and a LCD display.

The motor's transfer function was obtained via experimental methods and used in Matlab/Simulink to tune a Fuzzy-logic controller. The fuzzy logic controller was then implemented in C in a platform-abstract way.

Make sure to import the project as a DAVE CE project.
