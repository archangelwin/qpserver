#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//登录命令

#define MDM_GP_LOGON        1                 //广场登录

//登录模式
#define SUB_GP_LOGON_GAMEID     1                 //I D 登录
#define SUB_GP_LOGON_ACCOUNTS   2                 //帐号登录
#define SUB_GP_REGISTER_ACCOUNTS  3                 //注册帐号
#define SUB_GP_LOGON_MANAGETOOL   4                 //管理工具
#define SUB_GP_VERIFY_INDIVIDUAL  5                 //验证资料
#define SUB_GP_LOGON_VISITOR    6                 //游客登录
#define SUB_GP_QUERY_VERIFY_CODE  7                 //查询验证码

//登录结果
#define SUB_GP_LOGON_SUCCESS    100                 //登录成功
#define SUB_GP_LOGON_FAILURE    101                 //登录失败
#define SUB_GP_LOGON_FINISH     102                 //登录完成
#define SUB_GP_VALIDATE_MBCARD      103                                 //登录失败
#define SUB_GP_VALIDATE_PASSPORT  104                 //登录失败  
#define SUB_GP_VERIFY_RESULT    105                 //验证结果
#define SUB_GP_MATCH_SIGNUPINFO   106                 //报名信息
#define SUB_GP_GROWLEVEL_CONFIG   107                 //等级配置
#define SUB_GP_VERIFY_CODE_RESULT 108                 //验证结果

//升级提示
#define SUB_GP_UPDATE_NOTIFY    200                 //升级提示

//////////////////////////////////////////////////////////////////////////////////
//
#define MB_VALIDATE_FLAGS           0x01                                //效验密保
#define LOW_VER_VALIDATE_FLAGS      0x02                                //效验低版本

//校验掩码
#define VERIFY_ACCOUNTS       0x01                //校验账号
#define VERIFY_NICKNAME       0x02                //校验昵称

//I D 登录
struct CMD_GP_LogonGameID
{
  //系统信息
  DWORD             dwPlazaVersion;           //广场版本
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列

  //登录信息
  DWORD             dwGameID;             //游戏 I D
  TCHAR             szPassword[LEN_MD5];        //登录密码
  BYTE              cbValidateFlags;              //校验标识
};

//帐号登录
struct CMD_GP_LogonAccounts
{
  //系统信息
  DWORD             dwPlazaVersion;           //广场版本
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列

  //登录信息
  BYTE              cbValidateFlags;              //校验标识
  TCHAR             szPassword[LEN_MD5];        //登录密码
  TCHAR             szAccounts[LEN_ACCOUNTS];     //登录帐号
  TCHAR             szPassPortID[LEN_PASS_PORT_ID];   //身份证号
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
  //系统信息
  DWORD             dwPlazaVersion;           //广场版本
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列

  //密码变量
  TCHAR             szLogonPass[LEN_MD5];       //登录密码

  //注册信息
  WORD              wFaceID;              //头像标识
  BYTE              cbGender;             //用户性别
  TCHAR             szAccounts[LEN_ACCOUNTS];     //登录帐号
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
  TCHAR             szSpreader[LEN_ACCOUNTS];     //推荐帐号
  TCHAR             szQQNumber[LEN_QQ];         //QQ 号码
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //手机号码
  TCHAR             szVerifyCode[LEN_VERIFY_CODE];    //验证码
  BYTE              cbValidateFlags;              //校验标识
};

//验证资料
struct CMD_GP_VerifyIndividual
{
  //系统信息
  DWORD             dwPlazaVersion;           //广场版本

  //验证信息
  WORD              wVerifyMask;            //验证掩码
};

//游客登录
struct CMD_GP_LogonVisitor
{
  DWORD             dwPlazaVersion;           //广场版本
  BYTE              cbValidateFlags;              //校验标识

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
};

//登录成功
struct CMD_GP_LogonSuccess
{
  //属性资料
  WORD              wFaceID;              //头像标识
  DWORD             dwUserID;             //用户 I D
  DWORD             dwGameID;             //游戏 I D
  DWORD             dwGroupID;              //社团标识
  DWORD             dwCustomID;             //自定标识
  DWORD             dwExperience;           //经验数值
  DWORD             dwLoveLiness;           //用户魅力

  //用户成绩
  SCORE             lUserScore;             //用户金币
  SCORE             lUserInsure;            //用户银行
  SCORE             lUserIngot;             //用户元宝
  DOUBLE              dUserBeans;             //用户游戏豆

  //用户信息
  BYTE              cbGender;             //用户性别
  BYTE              cbMoorMachine;            //锁定机器
  TCHAR             szAccounts[LEN_ACCOUNTS];     //登录帐号
  TCHAR             szNickName[LEN_ACCOUNTS];     //用户昵称
  TCHAR             szDynamicPass[LEN_PASSWORD];    //动态密码
  TCHAR             szGroupName[LEN_GROUP_NAME];    //社团名字

  //配置信息
  BYTE              cbInsureEnabled;          //银行使能标识
  BYTE                            cbShowServerStatus;                 //显示服务器状态
};

//登录失败
struct CMD_GP_LogonFailure
{
  LONG              lResultCode;            //错误代码
  TCHAR             szDescribeString[128];        //描述消息
};

//登陆完成
struct CMD_GP_LogonFinish
{
  WORD              wIntermitTime;            //中断时间
  WORD              wOnLineCountTime;         //更新时间
};

//登录失败
struct CMD_GP_ValidateMBCard
{
  UINT                uMBCardID;            //机器序列
};

//验证结果
struct CMD_GP_VerifyIndividualResult
{
  bool              bVerifyPassage;           //验证通过
  WORD              wVerifyMask;            //验证掩码
  TCHAR             szErrorMsg[128];          //错误信息
};

//升级提示
struct CMD_GP_UpdateNotify
{
  BYTE              cbMustUpdate;           //强行升级
  BYTE              cbAdviceUpdate;           //建议升级
  DWORD             dwCurrentVersion;         //当前版本
};

//查询验证码
struct CMD_GP_QueryVerifyCode
{
  bool                bCloseSocket;         //断网标识
  TCHAR               szMachineID[LEN_MACHINE_ID];  //机器码 (C2S)
  TCHAR               szVerifyCode[LEN_VERIFY_CODE];  //验证码 (S2C)
};

//验证结果
struct CMD_GP_VerifyCodeResult
{
  BYTE                cbResultCode;         //结果标识（1成功，0失败，2重复申请）
  TCHAR               szDescString[64];       //结果字符串
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO     1                 //社团信息
#define DTP_GP_MEMBER_INFO      2                 //会员信息
#define DTP_GP_UNDER_WRITE      3                 //个性签名
#define DTP_GP_STATION_URL      4                 //主页信息

//社团信息
struct DTP_GP_GroupInfo
{
  DWORD             dwGroupID;              //社团索引
  TCHAR             szGroupName[LEN_GROUP_NAME];    //社团名字
};

//会员信息
struct DTP_GP_MemberInfo
{
  BYTE              cbMemberOrder;            //会员等级
  SYSTEMTIME            MemberOverDate;           //到期时间
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_GP_SERVER_LIST      2                 //列表信息

//获取命令
#define SUB_GP_GET_LIST       1                 //获取列表
#define SUB_GP_GET_SERVER     2                 //获取房间
#define SUB_GP_GET_MATCH      3                 //获取比赛
#define SUB_GP_GET_ONLINE     4                 //获取在线
#define SUB_GP_GET_COLLECTION   5                 //获取收藏

//列表信息
#define SUB_GP_LIST_TYPE      100                 //类型列表
#define SUB_GP_LIST_KIND      101                 //种类列表
#define SUB_GP_LIST_NODE      102                 //节点列表
#define SUB_GP_LIST_PAGE      103                 //定制列表
#define SUB_GP_LIST_SERVER      104                 //房间列表
#define SUB_GP_LIST_MATCH     105                 //比赛列表
#define SUB_GP_VIDEO_OPTION     106                 //视频配置
#define SUB_GP_TRANS_ADDRESS    107                 //中转地址  

//完成信息
#define SUB_GP_LIST_FINISH      200                 //发送完成
#define SUB_GP_SERVER_FINISH    201                 //房间完成
#define SUB_GP_MATCH_FINISH     202                 //比赛完成

//在线信息
#define SUB_GR_KINE_ONLINE      300                 //类型在线
#define SUB_GR_SERVER_ONLINE    301                 //房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_GP_GetOnline
{
  WORD              wServerCount;           //房间数目
  WORD              wOnLineServerID[MAX_SERVER];    //房间标识
};

//类型在线
struct CMD_GP_KindOnline
{
  WORD              wKindCount;             //类型数目
  tagOnLineInfoKind       OnLineInfoKind[MAX_KIND];     //类型在线
};

//房间在线
struct CMD_GP_ServerOnline
{
  WORD              wServerCount;           //房间数目
  tagOnLineInfoServer       OnLineInfoServer[MAX_SERVER];   //房间在线
};

//////////////////////////////////////////////////////////////////////////////////
//服务命令

#define MDM_GP_USER_SERVICE       3               //用户服务

//账号服务
#define SUB_GP_MODIFY_MACHINE     100               //修改机器
#define SUB_GP_MODIFY_LOGON_PASS    101               //修改密码
#define SUB_GP_MODIFY_INSURE_PASS   102               //修改密码
#define SUB_GP_MODIFY_UNDER_WRITE   103               //修改签名

//修改头像
#define SUB_GP_USER_FACE_INFO     120               //头像信息
#define SUB_GP_SYSTEM_FACE_INFO     122               //系统头像
#define SUB_GP_CUSTOM_FACE_INFO     123               //自定头像

//个人资料
#define SUB_GP_USER_INDIVIDUAL      140               //个人资料
#define SUB_GP_QUERY_INDIVIDUAL     141               //查询信息
#define SUB_GP_MODIFY_INDIVIDUAL    152               //修改资料

//银行服务
#define SUB_GP_USER_ENABLE_INSURE   160               //开通银行
#define SUB_GP_USER_SAVE_SCORE      161               //存款操作
#define SUB_GP_USER_TAKE_SCORE      162               //取款操作
#define SUB_GP_USER_TRANSFER_SCORE    163               //转账操作
#define SUB_GP_USER_INSURE_INFO     164               //银行资料
#define SUB_GP_QUERY_INSURE_INFO    165               //查询银行
#define SUB_GP_USER_INSURE_SUCCESS    166               //银行成功
#define SUB_GP_USER_INSURE_FAILURE    167               //银行失败
#define SUB_GP_QUERY_USER_INFO_REQUEST  168               //查询用户
#define SUB_GP_QUERY_USER_INFO_RESULT 169               //用户信息
#define SUB_GP_USER_INSURE_ENABLE_RESULT 170              //开通结果
#define SUB_GP_USER_LOGON_INSURE    171               //登录银行

//比赛服务
#define SUB_GP_MATCH_SIGNUP       200               //比赛报名
#define SUB_GP_MATCH_UNSIGNUP     201               //取消报名
#define SUB_GP_MATCH_SIGNUP_RESULT    202               //报名结果

//签到服务
#define SUB_GP_CHECKIN_QUERY      220               //查询签到
#define SUB_GP_CHECKIN_INFO       221               //签到信息
#define SUB_GP_CHECKIN_DONE       222               //执行签到
#define SUB_GP_CHECKIN_RESULT     223               //签到结果

//任务服务
#define SUB_GP_TASK_LOAD        240               //加载任务
#define SUB_GP_TASK_TAKE        241               //领取任务
#define SUB_GP_TASK_REWARD        242               //任务奖励
#define SUB_GP_TASK_INFO        243               //任务信息
#define SUB_GP_TASK_LIST        244               //任务信息
#define SUB_GP_TASK_RESULT        245               //任务结果

//低保服务
#define SUB_GP_BASEENSURE_LOAD      260               //加载低保
#define SUB_GP_BASEENSURE_TAKE      261               //领取低保
#define SUB_GP_BASEENSURE_PARAMETER   262               //低保参数
#define SUB_GP_BASEENSURE_RESULT    263               //低保结果

//推广服务
#define SUB_GP_SPREAD_QUERY       280               //推广奖励
#define SUB_GP_SPREAD_INFO        281               //奖励参数

//等级服务
#define SUB_GP_GROWLEVEL_QUERY      300               //查询等级
#define SUB_GP_GROWLEVEL_PARAMETER    301               //等级参数
#define SUB_GP_GROWLEVEL_UPGRADE    302               //等级升级

//兑换服务
#define SUB_GP_EXCHANGE_QUERY     320               //兑换参数
#define SUB_GP_EXCHANGE_PARAMETER   321               //兑换参数
#define SUB_GP_PURCHASE_MEMBER      322               //购买会员
#define SUB_GP_PURCHASE_RESULT      323               //购买结果
#define SUB_GP_EXCHANGE_SCORE_BYINGOT 324               //兑换游戏币
#define SUB_GP_EXCHANGE_SCORE_BYBEANS 325               //兑换游戏币
#define SUB_GP_EXCHANGE_RESULT      326               //兑换结果

//抽奖服务
#define SUB_GP_LOTTERY_CONFIG_REQ   340               //请求配置
#define SUB_GP_LOTTERY_CONFIG     341               //抽奖配置
#define SUB_GP_LOTTERY_USER_INFO    342               //抽奖信息
#define SUB_GP_LOTTERY_START      343               //开始抽奖
#define SUB_GP_LOTTERY_RESULT     344               //抽奖结果

//游戏服务
#define SUB_GP_QUERY_USER_GAME_DATA   360               //查询数据                  

//操作结果
#define SUB_GP_OPERATE_SUCCESS      500               //操作成功
#define SUB_GP_OPERATE_FAILURE      501               //操作失败

//////////////////////////////////////////////////////////////////////////////////

//修改密码
struct CMD_GP_ModifyLogonPass
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szDesPassword[LEN_PASSWORD];    //用户密码
  TCHAR             szScrPassword[LEN_PASSWORD];    //用户密码
};

//修改密码
struct CMD_GP_ModifyInsurePass
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szDesPassword[LEN_PASSWORD];    //用户密码
  TCHAR             szScrPassword[LEN_PASSWORD];    //用户密码
};

//修改签名
struct CMD_GP_ModifyUnderWrite
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
  TCHAR             szUnderWrite[LEN_UNDER_WRITE];    //个性签名
};

//////////////////////////////////////////////////////////////////////////////////

//用户头像
struct CMD_GP_UserFaceInfo
{
  WORD              wFaceID;              //头像标识
  DWORD             dwCustomID;             //自定标识
};

//修改头像
struct CMD_GP_SystemFaceInfo
{
  WORD              wFaceID;              //头像标识
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//修改头像
struct CMD_GP_CustomFaceInfo
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
  DWORD             dwCustomFace[FACE_CX*FACE_CY];    //图片信息
};

//////////////////////////////////////////////////////////////////////////////////

//绑定机器
struct CMD_GP_ModifyMachine
{
  BYTE              cbBind;               //绑定标志
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//////////////////////////////////////////////////////////////////////////////////

//个人资料
struct CMD_GP_UserIndividual
{
  DWORD             dwUserID;             //用户 I D
  DWORD             lUserIngot;             //用户元宝
};

//查询信息
struct CMD_GP_QueryIndividual
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
};

//修改资料
struct CMD_GP_ModifyIndividual
{
  BYTE              cbGender;             //用户性别
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
};

//////////////////////////////////////////////////////////////////////////////////
//携带信息 CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS      1                 //用户账号  
#define DTP_GP_UI_NICKNAME      2                 //用户昵称
#define DTP_GP_UI_USER_NOTE     3                 //用户说明
#define DTP_GP_UI_UNDER_WRITE   4                 //个性签名
#define DTP_GP_UI_QQ        5                 //Q Q 号码
#define DTP_GP_UI_EMAIL       6                 //电子邮件
#define DTP_GP_UI_SEAT_PHONE    7                 //固定电话
#define DTP_GP_UI_MOBILE_PHONE    8                 //移动电话
#define DTP_GP_UI_COMPELLATION    9                 //真实名字
#define DTP_GP_UI_DWELLING_PLACE  10                  //联系地址
#define DTP_GP_UI_PASSPORT_ID   11                  //身份证号

//////////////////////////////////////////////////////////////////////////////////

//银行资料
struct CMD_GP_UserInsureInfo
{
  BYTE              cbEnjoinTransfer;         //转账开关
  WORD              wRevenueTake;           //税收比例
  WORD              wRevenueTransfer;         //税收比例
  WORD              wRevenueTransferMember;       //税收比例
  WORD              wServerID;              //房间标识
  SCORE             lUserScore;             //用户金币
  SCORE             lUserInsure;            //银行金币
  SCORE             lTransferPrerequisite;        //转账条件
};

//开通银行
struct CMD_GP_UserEnableInsure
{
  DWORD             dwUserID;             //用户I D
  TCHAR             szLogonPass[LEN_PASSWORD];      //登录密码
  TCHAR             szInsurePass[LEN_PASSWORD];     //银行密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//登录银行
struct CMD_GP_UserLogonInsure
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szInsurePass[LEN_MD5];        //银行密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//存入金币
struct CMD_GP_UserSaveScore
{
  DWORD             dwUserID;             //用户 I D
  SCORE             lSaveScore;             //存入金币
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//提取金币
struct CMD_GP_UserTakeScore
{
  DWORD             dwUserID;             //用户 I D
  SCORE             lTakeScore;             //提取金币
  TCHAR             szPassword[LEN_MD5];        //银行密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//转账金币
struct CMD_GP_UserTransferScore
{
  DWORD             dwUserID;             //用户 I D
  SCORE             lTransferScore;           //转账金币
  TCHAR             szPassword[LEN_MD5];        //银行密码
  TCHAR             szAccounts[LEN_NICKNAME];     //目标用户
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
  TCHAR             szTransRemark[LEN_TRANS_REMARK];  //转账备注
};

//银行成功
struct CMD_GP_UserInsureSuccess
{
  BYTE              cbOperateType;            //操作类型
  DWORD             dwUserID;             //用户 I D
  DWORD             dwUserMedal;            //用户奖牌
  DWORD             dwRecordIndex;            //记录编号
  SCORE             lUserScore;             //用户金币
  SCORE             lUserInsure;            //银行金币
  TCHAR             szDescribeString[128];        //描述消息
};

//银行失败
struct CMD_GP_UserInsureFailure
{
  LONG              lResultCode;            //错误代码
  TCHAR             szDescribeString[128];        //描述消息
};

//提取结果
struct CMD_GP_UserTakeResult
{
  DWORD             dwUserID;             //用户 I D
  SCORE             lUserScore;             //用户金币
  SCORE             lUserInsure;            //银行金币
};

//查询银行
struct CMD_GP_QueryInsureInfo
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_MD5];        //银行密码
};

//查询用户
struct CMD_GP_QueryUserInfoRequest
{
  BYTE                            cbByNickName;                       //昵称赠送
  TCHAR             szAccounts[LEN_ACCOUNTS];     //目标用户
};

//用户信息
struct CMD_GP_UserTransferUserInfo
{
  DWORD             dwTargetGameID;           //目标用户
  TCHAR             szAccounts[LEN_ACCOUNTS];     //目标用户
};

//开通结果
struct CMD_GP_UserInsureEnableResult
{
  BYTE              cbInsureEnabled;          //使能标识
  TCHAR             szDescribeString[128];        //描述消息
};

//////////////////////////////////////////////////////////////////////////////////
//查询签到
struct CMD_GP_CheckInQueryInfo
{
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //登录密码
};

//签到信息
struct CMD_GP_CheckInInfo
{
  WORD              wSeriesDate;            //连续日期
  bool              bTodayChecked;            //签到标识
  SCORE             lRewardGold[LEN_WEEK];        //奖励金币
};

//执行签到
struct CMD_GP_CheckInDone
{
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //登录密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//签到结果
struct CMD_GP_CheckInResult
{
  bool              bSuccessed;             //成功标识
  SCORE             lScore;               //当前金币
  TCHAR             szNotifyContent[128];       //提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//任务服务

//加载任务
struct CMD_GP_TaskLoadInfo
{
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码
};

//领取任务
struct CMD_GP_TaskTake
{
  WORD              wTaskID;              //任务标识
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //登录密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//领取奖励
struct CMD_GP_TaskReward
{
  WORD              wTaskID;              //任务标识
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //登录密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//任务信息
struct CMD_GP_TaskInfo
{
  WORD              wTaskCount;             //任务数量
  tagTaskStatus         TaskStatus[TASK_MAX_COUNT];     //任务状态
};

//任务结果
struct CMD_GP_TaskResult
{
  //结果信息
  bool              bSuccessed;             //成功标识
  WORD              wCommandID;             //命令标识

  //财富信息
  SCORE             lCurrScore;             //当前金币
  SCORE             lCurrIngot;             //当前元宝

  //提示信息
  TCHAR             szNotifyContent[128];       //提示内容
};


//////////////////////////////////////////////////////////////////////////////////
//低保服务

//领取低保
struct CMD_GP_BaseEnsureTake
{
  DWORD             dwUserID;             //用户 I D
  TCHAR             szPassword[LEN_PASSWORD];     //登录密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//低保参数
struct CMD_GP_BaseEnsureParamter
{
  SCORE             lScoreCondition;          //游戏币条件
  SCORE             lScoreAmount;           //游戏币数量
  BYTE              cbTakeTimes;            //领取次数
};

//低保结果
struct CMD_GP_BaseEnsureResult
{
  bool              bSuccessed;             //成功标识
  SCORE             lGameScore;             //当前游戏币
  TCHAR             szNotifyContent[128];       //提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//推广服务

//推广查询
struct CMD_GP_UserSpreadQuery
{
  DWORD             dwUserID;             //用户标识
};

//推广参数
struct CMD_GP_UserSpreadInfo
{
  DWORD             dwSpreadCount;            //推广人数
  SCORE             lSpreadReward;            //推广奖励
};

//////////////////////////////////////////////////////////////////////////////////
//等级服务

//查询等级
struct CMD_GP_GrowLevelQueryInfo
{
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_PASSWORD];     //用户密码

  //附加信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//等级配置
struct CMD_GP_GrowLevelConfig
{
  WORD              wLevelCount;            //等级数目
  tagGrowLevelConfig        GrowLevelItem[60];          //等级配置
};

//等级参数
struct CMD_GP_GrowLevelParameter
{
  WORD              wCurrLevelID;           //当前等级
  DWORD             dwExperience;           //当前经验
  DWORD             dwUpgradeExperience;        //下级经验
  SCORE             lUpgradeRewardGold;         //升级奖励
  SCORE             lUpgradeRewardIngot;        //升级奖励
};

//等级升级
struct CMD_GP_GrowLevelUpgrade
{
  SCORE             lCurrScore;             //当前游戏币
  SCORE             lCurrIngot;             //当前元宝
  TCHAR             szNotifyContent[128];       //提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//兑换服务

//查询参数
struct CMD_GP_ExchangeParameter
{
  DWORD             dwExchangeRate;           //元宝游戏币兑换比率
  DWORD             dwPresentExchangeRate;        //魅力游戏币兑换率
  DWORD             dwRateGold;             //游戏豆游戏币兑换率
  WORD              wMemberCount;           //会员数目
  tagMemberParameter        MemberParameter[10];        //会员参数
};

//购买会员
struct CMD_GP_PurchaseMember
{
  DWORD             dwUserID;             //用户标识
  BYTE              cbMemberOrder;            //会员标识
  WORD              wPurchaseTime;            //购买时间
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
};

//购买结果
struct CMD_GP_PurchaseResult
{
  bool              bSuccessed;             //成功标识
  BYTE              cbMemberOrder;            //会员系列
  SCORE             lCurrScore;             //当前游戏币
  DOUBLE              dCurrBeans;             //当前游戏豆
  TCHAR             szNotifyContent[128];       //提示内容
};

//兑换游戏币
struct CMD_GP_ExchangeScoreByIngot
{
  DWORD             dwUserID;             //用户标识
  SCORE             lExchangeIngot;           //元宝数量
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
};

//兑换游戏币
struct CMD_GP_ExchangeScoreByBeans
{
  DWORD             dwUserID;             //用户标识
  double              dExchangeBeans;           //游戏豆数量
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
};

//兑换结果
struct CMD_GP_ExchangeResult
{
  bool              bSuccessed;             //成功标识
  SCORE             lCurrScore;             //当前游戏币
  SCORE             lCurrIngot;             //当前元宝
  double              dCurrBeans;             //当前游戏豆
  TCHAR             szNotifyContent[128];       //提示内容
};

//////////////////////////////////////////////////////////////////////////////////
//抽奖服务

//请求配置
struct CMD_GP_LotteryConfigReq
{
  WORD              wKindID;              //游戏标识
  DWORD             dwUserID;             //用户标识
  TCHAR             szLogonPass[LEN_MD5];       //登录密码
};

//抽奖配置
struct CMD_GP_LotteryConfig
{
  WORD              wLotteryCount;            //奖项个数
  tagLotteryItem          LotteryItem[MAX_LOTTERY];     //奖项内容
};

//抽奖信息
struct CMD_GP_LotteryUserInfo
{
  BYTE              cbFreeCount;            //免费次数
  BYTE              cbAlreadyCount;           //已领次数
  WORD              wKindID;              //游戏标识
  DWORD             dwUserID;             //用户标识
  SCORE             lChargeFee;             //抽奖费用
};

//开始抽奖
struct CMD_GP_LotteryStart
{
  WORD              wKindID;              //游戏标识
  DWORD             dwUserID;             //用户标识
  TCHAR             szLogonPass[LEN_MD5];       //登录密码
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//抽奖结果
struct CMD_GP_LotteryResult
{
  bool              bWined;               //中奖标识
  WORD              wKindID;              //游戏标识
  DWORD             dwUserID;             //用户标识
  SCORE             lUserScore;             //用户分数
  DOUBLE              dUserBeans;             //用户游戏豆
  tagLotteryItem          LotteryItem;            //中奖内容
};

//////////////////////////////////////////////////////////////////////////////////
//游戏服务
struct CMD_GP_QueryUserGameData
{
  WORD              wKindID;              //游戏标识
  DWORD             dwUserID;             //用户标识
  TCHAR             szDynamicPass[LEN_MD5];       //用户密码
};

//附加信息
#define DTP_GP_UI_USER_GAME_DATA  1                 //游戏数据  

//////////////////////////////////////////////////////////////////////////////////
//比赛报名
struct CMD_GP_MatchSignup
{
  //比赛信息
  WORD              wServerID;              //房间标识
  DWORD             dwMatchID;              //比赛标识
  DWORD             dwMatchNO;              //比赛场次

  //用户信息
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_MD5];        //登录密码

  //机器信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//取消报名
struct CMD_GP_MatchUnSignup
{
  //比赛信息
  WORD              wServerID;              //房间标识
  DWORD             dwMatchID;              //比赛标识
  DWORD             dwMatchNO;              //比赛场次

  //用户信息
  DWORD             dwUserID;             //用户标识
  TCHAR             szPassword[LEN_MD5];        //登录密码

  //机器信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器序列
};

//报名结果
struct CMD_GP_MatchSignupResult
{
  bool              bSignup;              //报名标识
  bool              bSuccessed;             //成功标识
  WORD              wServerID;              //房间标识
  SCORE             lCurrScore;             //当前金币
  TCHAR             szDescribeString[128];        //描述信息
};

//////////////////////////////////////////////////////////////////////////////////

//操作失败
struct CMD_GP_OperateFailure
{
  LONG              lResultCode;            //错误代码
  TCHAR             szDescribeString[128];        //描述消息
};

//操作成功
struct CMD_GP_OperateSuccess
{
  LONG              lResultCode;            //操作代码
  TCHAR             szDescribeString[128];        //成功消息
};

//////////////////////////////////////////////////////////////////////////////////
//远程服务

#define MDM_GP_REMOTE_SERVICE   4                 //远程服务

//查找服务
#define SUB_GP_C_SEARCH_DATABASE  100                 //数据查找
#define SUB_GP_C_SEARCH_CORRESPOND  101                 //协调查找

//查找服务
#define SUB_GP_S_SEARCH_DATABASE  200                 //数据查找
#define SUB_GP_S_SEARCH_CORRESPOND  201                 //协调查找

//////////////////////////////////////////////////////////////////////////////////

//协调查找
struct CMD_GP_C_SearchCorrespond
{
  DWORD             dwGameID;             //游戏标识
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
};

//协调查找
struct CMD_GP_S_SearchCorrespond
{
  WORD              wUserCount;             //用户数目
  tagUserRemoteInfo       UserRemoteInfo[16];         //用户信息
};

//////////////////////////////////////////////////////////////////////////////////
//机器服务

#define MDM_GP_ANDROID_SERVICE    5                 //机器服务

//参数操作
#define SUB_GP_GET_PARAMETER    100                 //获取参数
#define SUB_GP_ADD_PARAMETER    101                 //添加参数
#define SUB_GP_MODIFY_PARAMETER 102                 //修改参数
#define SUB_GP_DELETE_PARAMETER 103                 //删除参数

//参数信息
#define SUB_GP_ANDROID_PARAMETER  200                 //机器参数    

//////////////////////////////////////////////////////////////////////////////////
//获取参数
struct CMD_GP_GetParameter
{
  WORD              wServerID;              //房间标识
};

//添加参数
struct CMD_GP_AddParameter
{
  WORD              wServerID;              //房间标识
  tagAndroidParameter       AndroidParameter;         //机器参数
};

//修改参数
struct CMD_GP_ModifyParameter
{
  WORD              wServerID;              //房间标识
  tagAndroidParameter       AndroidParameter;         //机器参数
};

//删除参数
struct CMD_GP_DeleteParameter
{
  WORD              wServerID;              //房间标识
  DWORD             dwBatchID;              //批次标识
};

//////////////////////////////////////////////////////////////////////////////////
//机器参数
struct CMD_GP_AndroidParameter
{
  WORD              wSubCommdID;            //子命令码
  WORD              wParameterCount;          //参数数目
  tagAndroidParameter       AndroidParameter[MAX_BATCH];    //机器参数
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//登录命令
#define MDM_MB_LOGON        100                 //广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID     1                 //I D 登录
#define SUB_MB_LOGON_ACCOUNTS   2                 //帐号登录
#define SUB_MB_REGISTER_ACCOUNTS  3                 //注册帐号
#define SUB_MB_LOGON_OTHERPLATFORM  4                 //其他登录
#define SUB_MB_LOGON_VISITOR    5                 //游客登录

//登录结果
#define SUB_MB_LOGON_SUCCESS    100                 //登录成功
#define SUB_MB_LOGON_FAILURE    101                 //登录失败
#define SUB_MB_MATCH_SIGNUPINFO   102                 //报名信息

//升级提示
#define SUB_MB_UPDATE_NOTIFY    200                 //升级提示

//////////////////////////////////////////////////////////////////////////////////

//I D 登录
struct CMD_MB_LogonGameID
{
  //系统信息
  WORD              wModuleID;              //模块标识
  DWORD             dwPlazaVersion;           //广场版本
  BYTE                            cbDeviceType;                       //设备类型

  //登录信息
  DWORD             dwGameID;             //游戏 I D
  TCHAR             szPassword[LEN_MD5];        //登录密码

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //电话号码
};

//帐号登录
struct CMD_MB_LogonAccounts
{
  //系统信息
  WORD              wModuleID;              //模块标识
  DWORD             dwPlazaVersion;           //广场版本
  BYTE                            cbDeviceType;                       //设备类型

  //登录信息
  TCHAR             szPassword[LEN_MD5];        //登录密码
  TCHAR             szAccounts[LEN_ACCOUNTS];     //登录帐号

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //电话号码
};

//帐号登录
struct CMD_MB_LogonOtherPlatform
{
  //系统信息
  WORD              wModuleID;              //模块标识
  DWORD             dwPlazaVersion;           //广场版本
  BYTE                            cbDeviceType;                       //设备类型

  //登录信息
  BYTE              cbGender;             //用户性别
  BYTE              cbPlatformID;           //平台编号
  TCHAR             szUserUin[LEN_USER_UIN];      //用户Uin
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
  TCHAR             szCompellation[LEN_COMPELLATION]; //真实名字

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //电话号码
};

//注册帐号
struct CMD_MB_RegisterAccounts
{
  //系统信息
  WORD              wModuleID;              //模块标识
  DWORD             dwPlazaVersion;           //广场版本
  BYTE                            cbDeviceType;                       //设备类型

  //密码变量
  TCHAR             szLogonPass[LEN_MD5];       //登录密码

  //注册信息
  WORD              wFaceID;              //头像标识
  BYTE              cbGender;             //用户性别
  TCHAR             szAccounts[LEN_ACCOUNTS];     //登录帐号
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //电话号码
};

//游客登录
struct CMD_MB_LogonVisitor
{
  //系统信息
  WORD              wModuleID;              //模块标识
  DWORD             dwPlazaVersion;           //广场版本
  BYTE                            cbDeviceType;                       //设备类型

  //连接信息
  TCHAR             szMachineID[LEN_MACHINE_ID];    //机器标识
  TCHAR             szMobilePhone[LEN_MOBILE_PHONE];  //电话号码
};

//登录成功
struct CMD_MB_LogonSuccess
{
  WORD              wFaceID;              //头像标识
  BYTE              cbGender;             //用户性别
  BYTE              cbMoorMachine;            //锁定机器
  DWORD             dwCustomID;             //自定头像
  DWORD             dwUserID;             //用户 I D
  DWORD             dwGameID;             //游戏 I D
  DWORD             dwExperience;           //经验数值
  DWORD             dwLoveLiness;           //用户魅力
  TCHAR             szAccounts[LEN_ACCOUNTS];     //用户帐号
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
  TCHAR             szDynamicPass[LEN_PASSWORD];    //动态密码

  //财富信息
  SCORE             lUserScore;             //用户游戏币
  SCORE             lUserIngot;             //用户元宝
  SCORE             lUserInsure;            //用户银行
  DOUBLE              dUserBeans;             //用户游戏豆

  //扩展信息
  BYTE              cbInsureEnabled;          //使能标识
};

//登录失败
struct CMD_MB_LogonFailure
{
  LONG              lResultCode;            //错误代码
  TCHAR             szDescribeString[128];        //描述消息
};

//升级提示
struct CMD_MB_UpdateNotify
{
  BYTE              cbMustUpdate;           //强行升级
  BYTE              cbAdviceUpdate;           //建议升级
  DWORD             dwCurrentVersion;         //当前版本
};

//////////////////////////////////////////////////////////////////////////////////
//列表命令

#define MDM_MB_SERVER_LIST      101                 //列表信息

//列表信息
#define SUB_MB_LIST_KIND      100                 //种类列表
#define SUB_MB_LIST_SERVER      101                 //房间列表
#define SUB_MB_LIST_MATCH     102                 //比赛列表
#define SUB_MB_TRANS_ADDRESS    103                 //中转地址  
#define SUB_MB_LIST_FINISH      200                 //列表完成

#define SUB_MB_GET_ONLINE     300                 //获取在线
#define SUB_MB_KINE_ONLINE      301                 //类型在线
#define SUB_MB_SERVER_ONLINE    302                 //房间在线

//////////////////////////////////////////////////////////////////////////////////

//获取在线
struct CMD_MB_GetOnline
{
  WORD              wServerCount;           //房间数目
  WORD              wOnLineServerID[MAX_SERVER];    //房间标识
};

//类型在线
struct CMD_MB_KindOnline
{
  WORD              wKindCount;             //类型数目
  tagOnLineInfoKind       OnLineInfoKind[MAX_KIND];     //类型在线
};

//房间在线
struct CMD_MB_ServerOnline
{
  WORD              wServerCount;           //房间数目
  tagOnLineInfoServer       OnLineInfoServer[MAX_SERVER];   //房间在线
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif