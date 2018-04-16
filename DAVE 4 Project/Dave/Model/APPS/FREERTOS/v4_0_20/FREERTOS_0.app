<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="FREERTOS" URI="http://resources/4.0.20/app/FREERTOS/0" description="FreeRTOS is an open source real-time operating system (RTOS) for embedded systems" version="4.0.20" minDaveVersion="4.0.0" instanceLabel="FREERTOS_0" appLabel="">
  <upwardMapList xsi:type="ResourceModel:RequiredApp" href="../../GUI_SEGGERLIBRARY/v4_1_10/GUI_SEGGERLIBRARY_0.app#//@requiredApps.2"/>
  <properties singleton="true" sharable="true"/>
  <requiredApps URI="http://resources/4.0.20/app/FREERTOS/0/appres_clock" requiredAppName="CLOCK_XMC4" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../CLOCK_XMC4/v4_0_22/CLOCK_XMC4_0.app#/"/>
  </requiredApps>
  <requiredApps URI="http://resources/4.0.20/app/FREERTOS/0/appres_cpu" requiredAppName="CPU_CTRL_XMC4" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../CPU_CTRL_XMC4/v4_0_14/CPU_CTRL_XMC4_0.app#/"/>
  </requiredApps>
  <hwResources name="systick" URI="http://resources/4.0.20/app/FREERTOS/0/hwres_systick" resourceGroupUri="peripheral/cpu/0/systick" mResGrpUri="peripheral/cpu/0/systick">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/cpu/cpu_0.dd#//@provided.15"/>
  </hwResources>
  <hwResources name="excp_systick" URI="http://resources/4.0.20/app/FREERTOS/0/hwres_exception_systick" resourceGroupUri="peripheral/cpu/0/exception/systick" mResGrpUri="peripheral/cpu/0/exception/systick">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/cpu/cpu_0.dd#//@provided.14"/>
  </hwResources>
  <hwResources name="excp_pendsv" URI="http://resources/4.0.20/app/FREERTOS/0/hwres_exception_pendsv" resourceGroupUri="peripheral/cpu/0/exception/pendsv" mResGrpUri="peripheral/cpu/0/exception/pendsv">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/cpu/cpu_0.dd#//@provided.16"/>
  </hwResources>
  <hwResources name="excp_svcall" URI="http://resources/4.0.20/app/FREERTOS/0/hwres_exception_svcall" resourceGroupUri="peripheral/cpu/0/exception/svcall" mResGrpUri="peripheral/cpu/0/exception/svcall">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/cpu/cpu_0.dd#//@provided.13"/>
  </hwResources>
</ResourceModel:App>
