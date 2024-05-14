void* wmi_init(void* devt) { // wmi.c:155
    struct wmi_t* wmip; // r0 - :158
}
void wmi_qos_state_init(struct wmi_t* wmip) { // wmi.c:174
    unsigned char i; // r3 - :177
}
void wmi_set_control_ep(struct wmi_t* wmip, enum _enum_379028 eid) { // wmi.c:198
}
void wmi_shutdown(struct wmi_t* wmip) { // wmi.c:211
}
enum _enum_379253 wmi_dix_2_dot3(void* osbuf) { // wmi.c:225
    unsigned char* datap; // r5 - :228
    unsigned short typeorlen; // r6 - :229
    ATH_MAC_HDR macHdr; // None - :230
    ATH_LLC_SNAP_HDR* llcHdr; // r0 - :231
}
enum _enum_379253 wmi_data_hdr_add(void* osbuf, unsigned char msgType) { // wmi.c:297
    WMI_DATA_HDR* dtHdr; // r0 - :300
}
unsigned char wmi_implicit_create_pstream(struct wmi_t* wmip, void* osbuf, unsigned char dir, unsigned char up) { // wmi.c:315
    unsigned char* datap; // r5 - :317
    unsigned char userPriority; // r0 - :318
    unsigned char trafficClass; // r6 - :318
    ATH_LLC_SNAP_HDR* llcHdr; // r0 - :319
    unsigned short ipType; // r0 - :320
    WMI_DATA_HDR* dtHdr; // r0 - :321
    WMI_CREATE_PSTREAM_CMD cmd; // None - :322
    int streamExists; // r4 - :323
}
enum _enum_379253 wmi_data_hdr_remove(void* osbuf) { // wmi.c:397
}
enum _enum_379253 wmi_control_rx_xtnd(struct wmi_t* wmip, void* osbuf) { // wmi.c:409
    WMIX_CMD_HDR* cmd; // r0 - :412
    unsigned short id; // r0 - :413
    unsigned char* datap; // r0 - :414
    unsigned long len; // r0 - :415
    enum _enum_379253 status; // r0 - :416
}
enum _enum_379253 wmi_control_rx(struct wmi_t* wmip, void* osbuf) { // wmi.c:483
    WMI_CMD_HDR* cmd; // r0 - :486
    unsigned short id; // r4 - :487
    unsigned char* datap; // r0 - :488
    unsigned long len; // r5 - :489
    enum _enum_379253 status; // r0 - :490
    unsigned char freeFlag; // r10 - :491
}
enum _enum_379253 wmi_cmd_send(struct wmi_t* wmip, void* osbuf, enum _enum_380225 cmdId, enum _enum_382565 syncflag) { // wmi.c:1253
    WMI_CMD_HDR* cHdr; // r0 - :1258
    enum _enum_379028 eid; // r5 - :1259
}
enum _enum_379253 wmi_cmd_send_xtnd(struct wmi_t* wmip, void* osbuf, enum _enum_382711 cmdId, enum _enum_382565 syncflag) { // wmi.c:1302
    WMIX_CMD_HDR* cHdr; // r0 - :1306
}
enum _enum_379253 wmi_connect_cmd(struct wmi_t* wmip, enum _enum_383036 netType, enum _enum_383127 dot11AuthMode, enum _enum_383190 authMode, enum _enum_383330 pairwiseCrypto, unsigned char pairwiseCryptoLen, enum _enum_383330 groupCrypto, unsigned char groupCryptoLen, int ssidLength, unsigned char* ssid, unsigned char* bssid, unsigned short channel, unsigned long ctrl_flags) { // wmi.c:1318
    void* osbuf; // r5 - :1326
    WMI_CONNECT_CMD* cc; // r0 - :1327
}
enum _enum_379253 wmi_reconnect_cmd(struct wmi_t* wmip, unsigned char* bssid, unsigned short channel) { // wmi.c:1368
    void* osbuf; // r6 - :1371
    WMI_RECONNECT_CMD* cc; // r0 - :1372
}
enum _enum_379253 wmi_disconnect_cmd(struct wmi_t* wmip) { // wmi.c:1393
    void* osbuf; // r0 - :1396
    enum _enum_379253 status; // r0 - :1397
}
enum _enum_379253 wmi_startscan_cmd(struct wmi_t* wmip, enum _enum_383926 scanType, int forceFgScan, int isLegacy, unsigned long homeDwellTime, unsigned long forceScanInterval, unsigned char numChan, unsigned short* channelList) { // wmi.c:1412
    void* osbuf; // r4 - :1418
    WMI_START_SCAN_CMD* sc; // r0 - :1419
    signed char size; // r5 - :1420
}
enum _enum_379253 wmi_scanparams_cmd(struct wmi_t* wmip, unsigned short fg_start_sec, unsigned short fg_end_sec, unsigned short bg_sec, unsigned short minact_chdw_msec, unsigned short maxact_chdw_msec, unsigned short pas_chdw_msec, unsigned char shScanRatio, unsigned char scanCtrlFlags, unsigned long max_dfsch_act_time) { // wmi.c:1456
    void* osbuf; // r0 - :1464
    WMI_SCAN_PARAMS_CMD* sc; // r0 - :1465
}
enum _enum_379253 wmi_bssfilter_cmd(struct wmi_t* wmip, unsigned char filter, unsigned long ieMask) { // wmi.c:1490
    void* osbuf; // r0 - :1493
    WMI_BSS_FILTER_CMD* cmd; // r0 - :1494
}
enum _enum_379253 wmi_probedSsid_cmd(struct wmi_t* wmip, unsigned char index, unsigned char flag, unsigned char ssidLength, unsigned char* ssid) { // wmi.c:1516
    void* osbuf; // r0 - :1520
    WMI_PROBED_SSID_CMD* cmd; // r0 - :1521
}
enum _enum_379253 wmi_powermode_cmd(struct wmi_t* wmip, unsigned char powerMode) { // wmi.c:1624
    void* osbuf; // r0 - :1627
    WMI_POWER_MODE_CMD* cmd; // r0 - :1628
}
enum _enum_379253 wmi_disctimeout_cmd(struct wmi_t* wmip, unsigned char timeout) { // wmi.c:1695
    void* osbuf; // r0 - :1698
    WMI_DISC_TIMEOUT_CMD* cmd; // r0 - :1699
}
enum _enum_379253 wmi_addKey_cmd(struct wmi_t* wmip, unsigned char keyIndex, enum _enum_383330 keyType, unsigned char keyUsage, unsigned char keyLength, unsigned char* keyRSC, unsigned char* keyMaterial, unsigned char key_op_ctrl, enum _enum_382565 sync_flag) { // wmi.c:1716
    void* osbuf; // r4 - :1722
    WMI_ADD_CIPHER_KEY_CMD* cmd; // r5 - :1723
}
enum _enum_379253 wmi_add_krk_cmd(struct wmi_t* wmip, unsigned char* krk) { // wmi.c:1757
    void* osbuf; // r0 - :1760
    WMI_ADD_KRK_CMD* cmd; // r0 - :1761
}
enum _enum_379253 wmi_deleteKey_cmd(struct wmi_t* wmip, unsigned char keyIndex) { // wmi.c:1792
    void* osbuf; // r0 - :1795
    WMI_DELETE_CIPHER_KEY_CMD* cmd; // r0 - :1796
}
enum _enum_379253 wmi_set_tkip_countermeasures_cmd(struct wmi_t* wmip, int en) { // wmi.c:1852
    void* osbuf; // r4 - :1855
    WMI_SET_TKIP_COUNTERMEASURES_CMD* cmd; // r0 - :1856
}
enum _enum_379253 wmi_dataSync_send(struct wmi_t* wmip, void* osbuf, enum _enum_379028 eid) { // wmi.c:1872
    WMI_DATA_HDR* dtHdr; // r0 - :1875
}
static enum _enum_379253 wmi_sync_point(struct wmi_t* wmip) { // wmi.c:1898
    void* cmd_osbuf; // r6 - :1901
    WMI_SYNC_CMD* cmd; // r0 - :1902
    struct _WMI_DATA_SYNC_BUFS dataSyncBufs[4]; // None - :1903
    unsigned char numPriStreams; // r7 - :1904
    unsigned char i; // r2 - :1904
    enum _enum_379253 status; // r8 - :1905
}
enum _enum_379253 wmi_create_pstream_cmd(struct wmi_t* wmip, WMI_CREATE_PSTREAM_CMD* params) { // wmi.c:2006
    void* osbuf; // r5 - :2009
    WMI_CREATE_PSTREAM_CMD* cmd; // r0 - :2010
    unsigned short activeTsids; // r0 - :2011
    unsigned char fatPipeExistsForAC; // r4 - :2012
}
enum _enum_379253 wmi_set_framerate_cmd(struct wmi_t* wmip, unsigned char bEnable, unsigned char type, unsigned char subType, unsigned short rateMask) { // wmi.c:2140
    void* osbuf; // r0 - :2143
    WMI_FRAME_RATES_CMD* cmd; // r0 - :2144
}
enum _enum_379253 wmi_set_bitrate_cmd(struct wmi_t* wmip, long dataRate, long mgmtRate, long ctlRate) { // wmi.c:2178
    void* osbuf; // r0 - :2181
    WMI_BIT_RATE_CMD* cmd; // r0 - :2182
    signed char crix; // r6 - :2183
    signed char mrix; // r5 - :2183
    signed char drix; // r4 - :2183
}
static int wmi_is_bitrate_index_valid(struct wmi_t* wmip, unsigned long rateIndex) { // wmi.c:2247
    enum _enum_386791 phyMode; // r0 - :2250
    int isValid; // r0 - :2251
}
signed char wmi_validate_bitrate(struct wmi_t* wmip, long rate) { // wmi.c:2286
    signed char i; // r4 - :2289
}
enum _enum_379253 wmi_get_channelList_cmd(struct wmi_t* wmip) { // wmi.c:2359
    void* osbuf; // r0 - :2362
}
enum _enum_379253 wmi_set_channelParams_cmd(struct wmi_t* wmip, unsigned char scanParam, enum _enum_386791 mode, signed char numChan, unsigned short* channelList) { // wmi.c:2383
    void* osbuf; // r0 - :2388
    WMI_CHANNEL_PARAMS_CMD* cmd; // r0 - :2389
    signed char size; // r8 - :2390
}
enum _enum_379253 wmi_set_error_report_bitmask(struct wmi_t* wmip, unsigned long mask) { // wmi.c:2424
    void* osbuf; // r0 - :2427
    WMI_TARGET_ERROR_REPORT_BITMASK* cmd; // r0 - :2429
}
enum _enum_379253 wmi_get_challenge_resp_cmd(struct wmi_t* wmip, unsigned long cookie, unsigned long source) { // wmi.c:2449
    void* osbuf; // r0 - :2452
    WMIX_HB_CHALLENGE_RESP_CMD* cmd; // r0 - :2453
}
unsigned char wmi_get_power_mode_cmd(struct wmi_t* wmip) { // wmi.c:3109
}
enum _enum_379253 wmi_verify_tspec_params(WMI_CREATE_PSTREAM_CMD* pCmd, int tspecCompliance) { // wmi.c:3115
    enum _enum_379253 ret; // r3 - :3118
}
enum _enum_379253 wmi_set_keepalive_cmd(struct wmi_t* wmip, unsigned char keepaliveInterval) { // wmi.c:3344
    void* osbuf; // r0 - :3347
    WMI_SET_KEEPALIVE_CMD* cmd; // r0 - :3348
}
enum _enum_379253 wmi_set_wsc_status_cmd(struct wmi_t* wmip, unsigned char status) { // wmi.c:3395
    void* osbuf; // r0 - :3398
    unsigned char* data; // r0 - :3399
}
long wmi_get_rate(signed char rateindex) { // wmi.c:3433
}
enum _enum_379253 wmi_set_hb_timeout_cmd(struct wmi_t* wmip, unsigned long timeout) { // wmi.c:3443
    void* osbuf; // r0 - :3446
    WMI_SET_HBTIMEOUT_CMD* sc; // r0 - :3447
    enum _enum_379253 status; // r0 - :3448
}
enum _enum_379253 wmi_host_exit_notify_cmd(struct wmi_t* wmip) { // wmi.c:3464
    void* osbuf; // r0 - :3467
    enum _enum_379253 status; // r0 - :3468
}