<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="GUI_SEGGERLIBRARY" URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0" description="Provides an interface to create GUI applications which can run in RTOS/Non-RTOS environment." mode="NOTSHARABLE" version="4.1.10" minDaveVersion="4.0.0" instanceLabel="GUI_SEGGERLIBRARY_0" appLabel="">
  <properties singleton="true" provideInit="true"/>
  <virtualSignals name="GPIO pin" URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/vs_iopin" hwSignal="pad" hwResource="//@hwResources.0"/>
  <virtualSignals name="GPIO pin_signal" URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/__pin_vs_iopin" hwSignal="pin" hwResource="//@hwResources.1"/>
  <requiredApps URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/appres_systimer" requiredAppName="SYSTIMER" required="false" requiringMode="SHARABLE"/>
  <requiredApps URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/appres_spi_master" requiredAppName="SPI_MASTER" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../SPI_MASTER/v4_3_20/SPI_MASTER_1.app#/"/>
  </requiredApps>
  <requiredApps URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/appres_freertos" requiredAppName="FREERTOS" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../FREERTOS/v4_0_20/FREERTOS_0.app#/"/>
  </requiredApps>
  <requiredApps URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/appres_cmsisrtos" requiredAppName="CMSIS_RTOS_RTX" required="false" requiringMode="SHARABLE"/>
  <hwResources name="GPIO pin" URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/hwres_iopin" resourceGroupUri="port/p/*/pad/*" mResGrpUri="port/p/*/pad/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/port0/port0_0.dd#//@provided.10"/>
  </hwResources>
  <hwResources name="GPIO pin" URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/__pin_hwres_iopin" resourceGroupUri="devicepackage/0/138" constraintType="GLOBAL_RESOURCE" mResGrpUri="devicepackage/0/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/DEVICEPACKAGE/DEVICEPACKAGE_0.dd#//@provided.16"/>
  </hwResources>
  <connections URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/vs_iopin/http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/__pin_vs_iopin" systemDefined="true" sourceSignal="GPIO pin" targetSignal="GPIO pin_signal" srcVirtualSignal="//@virtualSignals.0" targetVirtualSignal="//@virtualSignals.1"/>
  <connections URI="http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/__pin_vs_iopin/http://resources/4.1.10/app/GUI_SEGGERLIBRARY/0/vs_iopin" systemDefined="true" sourceSignal="GPIO pin_signal" targetSignal="GPIO pin" srcVirtualSignal="//@virtualSignals.1" targetVirtualSignal="//@virtualSignals.0"/>
</ResourceModel:App>
