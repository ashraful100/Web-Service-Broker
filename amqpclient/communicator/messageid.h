#ifndef MESSAGEID_H
#define MESSAGEID_H


class MessageId
{
public:
    MessageId();
    ~MessageId();

    enum TpMessage{
        TP_STATUS=100,
        TP_WP_NEXT,
        TP_WP_REACHED,
        TP_BTW,
        TP_DTW,
        TP_JTW,
        TP_NUM_OF_WPS,
        TpStatusData,
        TP_REQUEST_UPDATE,
        TP_MAX_SPEED,
        TP_MIN_SPEED,
        TP_MAX_XTE,
        TP_CMD_VTG,
        TP_CMD_EOT,
        TP_WP_REACH_RADIUS,
        TP_PORT_XTD,
        TP_STARTBOARD_XTD,
        TP_SPEEDPILOT_SWITCH,
        TP_SPEEDPILOT_MODE,
        TP_XTE_WARNING

    };

    enum AnsStatus{
        TP_ON = 200,
        TP_OFF,
        TP_PAUSE,
        TP_ACTIVE,
        TP_INACTIVE,
        HWC_ON,
        HWC_OFF,
        HWC_ACTIVE,
        HWC_INACTIVE,
        CA_ON,
        CA_OFF,
        CA_ACTIVE,
        CA_INACTIVE
    };

    enum CaMessage{
        CaSettingsData = 300,
        OperationalEnvelopeData,
        OwnShipData,
        TrafficShipData,
        LogData,
        CaMaLogData,
        AlarmData,
        WarningData,
        CaStatusData,
        CaIndicatorFlag
    };



    enum HwcMessage{
        HwcStatusData = 400,
        HwcWaveData,
        HwcAutoPilotData,
        HwcCmd,
        HwcSettingsData,
        HwcShipStabilityData,
        HwcSendingCmd,
        LogDataHwc,
        AlarmDataHwc,
        WarningDataHwc
    };

    enum HwcSettings{
        HwclppValue = 500,
        HwcMinSpeedValue,
        HwcMaxSpeedValue,
        HwcBValue,
        HwcTValue,
        HwcGMValue,
        HwcKValue,
        HwcSpeedMaxEot,
        HwcMaxXteValue,
        HwcDefaultK,
        HWC_REQUEST_UPDATE,
        HwcPolarPlotRange,
    };

    enum OeType{
        invalid=0,
        radiusMonitoring=600,
        radiusAction,
        radiusMls,
        minCpa,
        actionMode,
        restrictedVisibilty,
        visibilityIsRestricted,
        minManeuveringSpeed,
        maxRudderCmd,
        clearAbaft,
        clearAhead,
        closeQuarterExits,
        osDomainOffsetDistance,
        osDomainOffsetAzimuth,
        osDomainRadius,
        safetyFactorWaypointPlanning,
        maxCrossTrackAspired,
        resetOsDomainToDefault,
        saveToConfig,
        oeSyncReq


    };

    enum TsData{
        navigationalStatusManual = 700,
        navigationalStatus,
        caHandling,
        osIsGiveWay,
        osIsGiveWayManual,
        domainOffsetDistance,
        domainOffsetAzimuth,
        domainRadius,
        caType,
        colregRule,
        cpa,
        tcpa,
        minDist,
        crRankNa,
        crRankOd,
        domainReset,
        showDomain,
        showTsDomains
    };

    enum OsData{
        osNavigationalStatusManual,
    };

//    enum CaType{
//        standard,
//        virtualSd,
//        predicted,
//        predictedSd,
//        predictedSdCog,
//        timeCorrected,
//        minDistStToSd,
//        givenOsPos,
//    };

    enum TsType{
        tsStandard = 800,
        tsPredicted,
        tsVirtualSd,
        tsTimeCorrected,
        tsMinDistStToSd,
        tsSc,
    };

    enum OsType{
        osStandard = 900,
        osGivenCog,
        osGivenSog,
        osGivenCogSog,
        osGivenOsPos,
        osCa,
        osCaGivenCog,
        osSc,
    };

    enum CaWp{
        insert = 1000,
        move
    };

    enum SdViolatedType{
        standOn= 1100,
        critTime,
        none
    };

    enum OsTurnTo{
        port = 1200,
        starboard
    };

    enum FaaIndicatorFlag{
        NavigationFlag=1300,
        ObjDetectionFlag,
        VoyageFlag
    };

    enum ScVoyageLegType{
        osNewTsNew=1400,
        osNewTsPred,
        osPredTsNew
    };

    enum ScShipType{
        scOs=1500,
        scTs,
    };

    enum AlarmType{
        Warning =1600,
        Critical,
        Information
    };

    enum AlarmSource{
        Source_VO=1700, // Voyage Controller CA
        Source_AO,  //Area Observation CA
        Source_RE, // Route Evaluator
        Source_SCM, // SC Map
        Source_StmMessageHandler
    };
};

#endif // MESSAGEID_H
