void nin_driver_init(struct ar_drv_init_param* init_param) { // ar6000_drv.c:379
    struct _HTC_INIT_INFO initInfo; // None - :383
    unsigned long oldpri; // r0 - :384
}
void nin_driver_exit() { // ar6000_drv.c:430
}
enum _enum_350801 ar6000_reset_device(struct hif_device* hifDevice, unsigned long resetValue) { // ar6000_drv.c:454
    enum _enum_350801 status; // r0 - :456
    unsigned long address; // None - :457
    unsigned long data; // None - :458
}
void nin_driver_destroy(void* device) { // ar6000_drv.c:502
    struct ar6_softc* ar; // r0 - :504
    struct _HTC_TARGET* htcTarget; // r6 - :505
}
enum _enum_350801 nin_driver_rom_patch_install(void* device, unsigned long compare_addr, unsigned long target_addr, unsigned long length, unsigned long do_activate, unsigned long* rompatchID) { // ar6000_drv.c:628
    enum _enum_350801 status; // r0 - :632
}
enum _enum_350801 nin_driver_rom_patch_active_deactive(void* device, unsigned char flag, unsigned long rompatch_count, unsigned long* rompatch_list) { // ar6000_drv.c:642
    struct ar6_softc* ar; // r0 - :644
    enum _enum_350801 status; // r0 - :645
}
long nin_driver_open(void* device) { // ar6000_drv.c:736
    struct ar6_softc* ar; // r0 - :738
}
void ar6000_ratemask_rx(void* devt, unsigned short ratemask) { // ar6000_drv.c:758
    struct ar6_softc* ar; // r0 - :761
}
long nin_driver_close(void* device) { // ar6000_drv.c:767
    struct ar6_softc* ar; // r0 - :769
}
long nin_driver_tx(void* netbuf, void* device) { // ar6000_drv.c:781
    struct ar6_softc* ar; // r0 - :785
    enum _enum_351940 endPointId; // r4 - :786
    unsigned long mapNo; // None - :787
    struct ar_cookie* cookie; // r5 - :788
    struct iphdr* ipHdr; // r0 - :789
    unsigned char ac; // r4 - :790
    int checkAdHocPsMapping; // r5 - :791
}
static enum _enum_350801 nin_connectservice(struct ar6_softc* ar, struct _HTC_SERVICE_CONNECT_REQ* pConnect) { // ar6000_drv.c:1010
    enum _enum_350801 status; // r5 - :1014
    struct _HTC_SERVICE_CONNECT_RESP response; // None - :1015
}
static void nin_avail_ev(void* HTCHandle) { // ar6000_drv.c:1066
    struct ar6_softc* ar; // r0 - :1069
    enum _enum_350801 status; // r0 - :1070
    unsigned long param; // None - :1071
    unsigned long oldsleep; // None - :1072
    unsigned long oldoptions; // None - :1073
    unsigned long chip_id; // None - :1074
    struct bmi_target_info targ_info; // None - :1156
    unsigned long chip_address; // None - :1157
}
static void nin_unavail_ev(void* Instance) { // ar6000_drv.c:1320
}
static enum _HTC_SEND_FULL_ACTION nin_tx_queue_full(void* Context, enum _enum_351940 Endpoint) { // ar6000_drv.c:1329
    struct ar6_softc* ar; // r0 - :1331
    enum _HTC_SEND_FULL_ACTION action; // r12 - :1332
    int stopNet; // r14 - :1333
}
static void nin_tx_complete(void* Context, struct _HTC_PACKET* pPacket) { // ar6000_drv.c:1387
    struct ar6_softc* ar; // r0 - :1390
    void* cookie; // r6 - :1391
    void* netbuf; // r7 - :1392
    unsigned long mapNo; // r4 - :1393
    enum _enum_350801 status; // r8 - :1394
    struct ar_cookie* ar_cookie; // r0 - :1395
    enum _enum_351940 eid; // r9 - :1396
    unsigned char needCb; // None - :1398
    unsigned long i; // r9 - :1455
}
static void nin_rx(void* Context, struct _HTC_PACKET* pPacket) { // ar6000_drv.c:1545
    struct ar6_softc* ar; // r0 - :1548
    void* netbuf; // r4 - :1549
    enum _enum_350801 status; // r5 - :1551
    enum _enum_351940 ept; // r6 - :1552
}
static void ar6000_detect_error(void* ptr) { // ar6000_drv.c:1786
    struct ar6_softc* ar; // r0 - :1789
    WMI_TARGET_ERROR_REPORT_EVENT errEvent; // None - :1790
}
static void ar6000_init_profile_info(struct ar6_softc* ar) { // ar6000_drv.c:1837
}
static void ar6000_install_static_wep_keys(struct ar6_softc* ar) { // ar6000_drv.c:1869
    unsigned char index; // r9 - :1872
    unsigned char keyUsage; // r0 - :1873
}
static struct ar_cookie* ar6000_alloc_cookie(struct ar6_softc* ar) { // ar6000_drv.c:1910
    struct ar_cookie* cookie; // r2 - :1913
}
unsigned char ar6000_ibss_map_epid(void* netbuf, struct ar6_softc* dev, unsigned long* mapNo) { // ar6000_drv.c:1924
    struct ar6_softc* ar; // r0 - :1927
    unsigned char* datap; // r0 - :1928
    ATH_MAC_HDR* macHdr; // r0 - :1929
    long emptMap; // r7 - :1930
    unsigned char i; // r8 - :1931
}
void ar6000_ready_event(void* devt, unsigned char* datap, unsigned char phyCap, unsigned long ramVersion) { // ar6000_drv.c:1983
    struct ar6_softc* ar; // r0 - :1985
}
unsigned char ar6000_iptos_to_userPriority(unsigned char* pkt) { // ar6000_drv.c:1999
}
enum _enum_350801 ar6000_control_tx(void* devt, void* osbuf, int endPt) { // ar6000_drv.c:2016
    struct ar6_softc* ar; // r0 - :2018
    struct ar_cookie* cookie; // r0 - :2020
}
void ar6000_connect_event(struct ar6_softc* ar, unsigned short channel, unsigned char* bssid) { // ar6000_drv.c:2056
}
void ar6000_disconnect_event(struct ar6_softc* ar, unsigned char reason, unsigned short protocolReasonStatus) { // ar6000_drv.c:2102
}
void ar6000_tkip_micerr_event(struct ar6_softc* ar, int ismcast) { // ar6000_drv.c:2160
    char buf[8]; // None - :2163
}
void ar6000_bitrate_rx(void* devt, long rateKbps) { // ar6000_drv.c:2175
    struct ar6_softc* ar; // r0 - :2177
}
void ar6000_channelList_rx(void* devt, signed char numChan, unsigned short* chanList) { // ar6000_drv.c:2183
    struct ar6_softc* ar; // r0 - :2185
}
void ar6000_regDomain_event(struct ar6_softc* ar, unsigned long regCode) { // ar6000_drv.c:2192
}
void ar6000_txPwr_rx(void* devt, unsigned char txPwr) { // ar6000_drv.c:2198
    struct ar6_softc* ar; // r0 - :2200
}
void ar6000_neighborReport_event(struct ar6_softc* ar, int numAps, WMI_NEIGHBOR_INFO* info) { // ar6000_drv.c:2206
    char buf[128]; // None - :2210
    int i; // r4 - :2211
}
void ar6000_set_numdataendpts(struct ar6_softc* ar, unsigned long num) { // ar6000_drv.c:2237
}
void ar6000_targetStats_event(struct ar6_softc* ar, WMI_TARGET_STATS* pTarget) { // ar6000_drv.c:2244
    struct targetStats_t* pStats; // r0 - :2246
    unsigned char ac; // r12 - :2247
}
void ar6000_hbChallengeResp_event(struct ar6_softc* ar, unsigned long cookie, unsigned long source) { // ar6000_drv.c:2318
}
void ar6000_reportError_event(enum _enum_361812 errorVal) { // ar6000_drv.c:2333
}
void ar6000_cac_event() { // ar6000_drv.c:2366
}
void ar6000_roam_data_event(WMI_TARGET_ROAM_DATA* p) { // ar6000_drv.c:2415
}
void ar6000_dset_open_req() { // ar6000_drv.c:2427
}
void ar6000_dset_close() { // ar6000_drv.c:2437
}
void ar6000_dset_data_req() { // ar6000_drv.c:2442
}
void ar6000_send_event_to_app(struct ar6_softc* ar, unsigned short eventId, unsigned char* datap, int len, void* cookie) { // ar6000_drv.c:2456
    void* netbuf; // r4 - :2459
    unsigned char* data; // r0 - :2461
}
enum _enum_350801 nin_drv_set_scan_params_cmd(void* device, unsigned short fg_start_period, unsigned short fg_end_period, unsigned short bg_period, unsigned short min_chdwell_time, unsigned short max_chdwell_time, unsigned short pas_chdwell_time, unsigned char shortScanRatio, unsigned char scanCtrlFlags) { // ar6000_drv.c:2520
    struct ar6_softc* ar; // r0 - :2528
    enum _enum_350801 ret; // r4 - :2529
}
enum _enum_350801 nin_drv_set_power_mode_cmd(void* device, unsigned char powerMode) { // ar6000_drv.c:2544
    struct ar6_softc* ar; // r0 - :2546
    enum _enum_350801 ret; // r4 - :2547
}
enum _enum_350801 nin_drv_set_frame_rate_cmd(void* device, unsigned char bEnable, unsigned char type, unsigned char subType, unsigned short rateMask) { // ar6000_drv.c:2574
    struct ar6_softc* ar; // r0 - :2576
    enum _enum_350801 ret; // r4 - :2577
}
enum _enum_350801 nin_drv_set_bit_rate_cmd(void* device, long dRatekbps, long mRatekbps, long cRatekbps) { // ar6000_drv.c:2589
    struct ar6_softc* ar; // r0 - :2591
    enum _enum_350801 ret; // r4 - :2592
}
enum _enum_350801 nin_drv_connect_cmd(void* device, unsigned char networkType, unsigned char ssidLength, unsigned char* ssid, unsigned short channel, unsigned char* bssid) { // ar6000_drv.c:2699
    struct ar6_softc* ar; // r0 - :2703
    enum _enum_350801 status; // r0 - :2705
    unsigned char ZERO_BSSID[6]; // None - :2706
}
enum _enum_350801 nin_drv_set_channel_params_cmd(void* device, unsigned char scanParam, unsigned char phyMode, unsigned char numChannels, unsigned short* channelList) { // ar6000_drv.c:2824
    struct ar6_softc* ar; // r0 - :2828
    enum _enum_350801 ret; // r4 - :2829
}
enum _enum_350801 nin_drv_set_bss_filter_cmd(void* device, unsigned char bssFilter) { // ar6000_drv.c:2856
    struct ar6_softc* ar; // r0 - :2858
    enum _enum_350801 ret; // r4 - :2859
}
enum _enum_350801 nin_drv_start_scan_cmd(void* device, unsigned char scanType, int forceFgScan, int isLegacy, unsigned long homeDwellTime, unsigned long forceScanInterval) { // ar6000_drv.c:2871
    struct ar6_softc* ar; // r0 - :2875
    enum _enum_350801 ret; // r4 - :2876
}
enum _enum_350801 nin_drv_disconnect_cmd(void* device) { // ar6000_drv.c:2896
    struct ar6_softc* ar; // r0 - :2898
    enum _enum_350801 ret; // r4 - :2899
}
enum _enum_350801 nin_drv_set_probed_ssid_cmd(void* device, unsigned char entryIndex, unsigned char flag, unsigned char ssidLength, unsigned char* ssid) { // ar6000_drv.c:2913
    struct ar6_softc* ar; // r0 - :2916
    enum _enum_350801 ret; // r4 - :2917
}
enum _enum_350801 nin_drv_delete_cipher_key_cmd(void* device, unsigned char keyIndex) { // ar6000_drv.c:2932
    struct ar6_softc* ar; // r0 - :2934
    enum _enum_350801 ret; // r4 - :2935
}
enum _enum_350801 nin_drv_set_disc_timeout_cmd(void* device, unsigned char disconnectTimeout) { // ar6000_drv.c:3038
    struct ar6_softc* ar; // r0 - :3040
    enum _enum_350801 ret; // r4 - :3041
}
enum _enum_350801 nin_drv_create_pstream_cmd(void* device, unsigned char trafficClass, unsigned char trafficDirection, unsigned char rxQueueNum, unsigned char voicePSCapability, unsigned char trafficType, unsigned char tsid, unsigned char userPriority, unsigned short nominalMSDU, unsigned short maxMSDU, unsigned long minServiceInt, unsigned long maxServiceInt, unsigned long inactivityInt, unsigned long minDataRate, unsigned long meanDataRate, unsigned long peakDataRate, unsigned long suspensionInt, unsigned long serviceStartTime, unsigned long sba, unsigned long mediumTime, unsigned long minPhyRate) { // ar6000_drv.c:3070
    struct ar6_softc* ar; // r0 - :3082
    enum _enum_350801 ret; // r0 - :3083
    WMI_CREATE_PSTREAM_CMD cmd; // None - :3084
}
enum _enum_350801 nin_drv_target_error_report_mask(void* device, unsigned long bitmask) { // ar6000_drv.c:3186
    struct ar6_softc* ar; // r0 - :3188
    enum _enum_350801 ret; // r4 - :3189
}
enum _enum_350801 nin_drv_get_channelList_cmd(void* device, unsigned char* numChannels, unsigned short* channelList) { // ar6000_drv.c:3220
    struct ar6_softc* ar; // r0 - :3223
    enum _enum_350801 ret; // r4 - :3224
}
enum _enum_350801 nin_drv_return_netbuf(void* netbuf) { // ar6000_drv.c:3345
}
enum _enum_350801 nin_drv_register_rxdata_handler(enum _enum_350801 (*cb)(void*, void*)) { // ar6000_drv.c:3351
}
enum _enum_350801 nin_drv_register_event_handler(enum _enum_350801 (*cb)(void*, void*)) { // ar6000_drv.c:3357
}
enum _enum_350801 nin_drv_register_txcompletion_handler(enum _enum_350801 (*cb)(void*, void*, enum _enum_350801)) { // ar6000_drv.c:3363
}
enum _enum_350801 nin_drv_register_image_info(unsigned long type, unsigned long patch_id, unsigned long start_addr, unsigned long length, unsigned long target_addr, unsigned long bCompressed) { // ar6000_drv.c:3385
}
enum _enum_350801 nin_drv_setparam(void* device, void* data) { // ar6000_drv.c:3587
    struct ar6_softc* ar; // r0 - :3590
    enum _enum_350801 ret; // r4 - :3591
    int* param; // r0 - :3592
    int value; // r5 - :3594
    int profChanged; // r6 - :3595
}
enum _enum_350801 nin_drv_setkey(void* device, void* data) { // ar6000_drv.c:3721
    struct ar6_softc* ar; // r0 - :3724
    struct ieee80211req_key* ik; // r6 - :3726
    enum _enum_363455 keyUsage; // r7 - :3727
    enum _enum_350801 status; // r6 - :3728
    enum _enum_354088 keyType; // r8 - :3729
}
enum _enum_350801 nin_drv_siwencode(void* device, void* data) { // ar6000_drv.c:4338
    struct ar6_softc* ar; // r0 - :4341
    int index; // r2 - :4342
    enum _enum_364295 auth; // r6 - :4343
    struct iw_point* erq; // r0 - :4344
}
enum _enum_350801 nin_drv_set_hb_challenge_resp_params(void* device, unsigned long frequency, unsigned long hostThreshold, unsigned long targetThreshold) { // ar6000_drv.c:4576
    struct ar6_softc* ar; // r0 - :4580
    enum _enum_350801 ret; // r4 - :4581
}
static void nin_device_error(void* Instance) { // ar6000_drv.c:4630
    struct ar6_softc* ar; // r0 - :4632
    WMI_TARGET_ERROR_REPORT_EVENT errEvent; // None - :4634
}
void ar6000_keepalive_rx(void* devt, unsigned char configured) { // ar6000_drv.c:4676
    struct ar6_softc* ar; // r0 - :4679
}
void ar6000_tx_retry_err_event() { // ar6000_drv.c:4685
}
void ar6000_roam_tbl_event(WMI_TARGET_ROAM_TBL* pTbl) { // ar6000_drv.c:4692
    unsigned char i; // r2 - :4695
}
void ar6000_gpio_intr_rx(unsigned long intr_mask, unsigned long input_values) { // ar6000_drv.c:4997
    struct ar6_softc* ar; // r0 - :5000
}
void ar6000_gpio_data_rx(unsigned long reg_id, unsigned long value) { // ar6000_drv.c:5013
    struct ar6_softc* ar; // r0 - :5016
}
void ar6000_gpio_ack_rx() { // ar6000_drv.c:5029
}
enum _enum_350801 nin_drv_set_wsc_status(void* dev, unsigned long wscStatus) { // ar6000_drv.c:5338
    struct ar6_softc* ar; // r0 - :5341
    enum _enum_350801 ret; // r4 - :5342
}
enum _enum_350801 ar6000_ReadRegDiag(struct hif_device* hifDevice, unsigned long* address, unsigned long* data) { // ar6000_drv.c:5354
    enum _enum_350801 status; // r0 - :5357
}
enum _enum_350801 ar6000_WriteRegDiag(struct hif_device* hifDevice, unsigned long* address, unsigned long* data) { // ar6000_drv.c:5401
    enum _enum_350801 status; // r0 - :5404
}
enum _enum_350801 ar6000_ReadDataDiag(struct hif_device* hifDevice, unsigned long address, unsigned char* data, unsigned long length) { // ar6000_drv.c:5444
    unsigned long count; // r5 - :5448
    enum _enum_350801 status; // r0 - :5449
}
enum _enum_350801 ar6000_WriteDataDiag(struct hif_device* hifDevice, unsigned long address, unsigned char* data, unsigned long length) { // ar6000_drv.c:5462
    unsigned long count; // r5 - :5466
    enum _enum_350801 status; // r0 - :5467
}
enum _enum_350801 ar6000_set_host_app_area(struct ar6_softc* ar) { // ar6000_drv.c:5490
    unsigned long data; // None - :5493
    unsigned long address; // None - :5493
    struct host_app_area_s host_app_area; // None - :5494
}
enum _enum_350801 nin_drv_CheckTargetReady(void* dev) { // ar6000_drv.c:5514
}
enum _enum_350801 nin_drv_BmiWriteMemory(void* dev, unsigned long address, unsigned long length, unsigned char* data) { // ar6000_drv.c:5557
}
enum _enum_350801 nin_download_file(unsigned long offset, unsigned char* buffer, unsigned long length, unsigned long target_addr, unsigned long bCompressed) { // ar6000_drv.c:5766
    unsigned int remaining; // r0 - :5771
}
enum _enum_350801 nin_set_htc_params(struct hif_device* hifDevice, unsigned long MboxIsrYieldValue, unsigned char HtcControlBuffers) { // ar6000_drv.c:5795
    enum _enum_350801 status; // r0 - :5799
    unsigned long blocksizes[4]; // None - :5800
}
enum _enum_351940 ar6000_ac2_endpoint_id(void* devt, unsigned char ac) { // ar6000_drv.c:5917
}
void ar6000_indicate_tx_activity(void* devt, unsigned char TrafficClass, int Active) { // ar6000_drv.c:5932
    struct ar6_softc* ar; // r0 - :5934
    enum _enum_351940 eid; // r3 - :5935
    int i; // r14 - :5936
}
static struct _HTC_PACKET* nin_ep_recv_alloc(enum _enum_351940 Endpoint, int Length) { // ar6000_drv.c:5989
    void* osBuf; // r0 - :5991
    struct _HTC_PACKET* pPacket; // r0 - :5992
}
static void ar6000_TxDataCleanup(struct ar6_softc* ar) { // ar6000_drv.c:6009
}
enum _enum_350801 ar6000_reset_force_host(struct hif_device* hifDevice) { // ar6000_drv.c:6257
    int i; // r4 - :6260
    struct forceROM_s* ForceROM; // r5 - :6265
}
static void ar6000_init_check(void* ptr) { // ar6000_drv.c:6314
    struct ar6_softc* ar; // r0 - :6316
}