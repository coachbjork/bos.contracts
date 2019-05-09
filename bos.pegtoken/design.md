# 锚定币合约设计方案  

# 第一部分：简介

锚定币合约为系统合约，在主网启动时部署在 BOS 链上，用于在 BOS 链上提供 USDT/BTC/ETH 与其主网的自由等比兑换。

为表述方便，定义一些名词：

```
角色：合约部署者（deployer）、承兑商(issuer)、承兑商相应币种承兑人（acceptor）、承兑商审核员（auditor）、普通用户(user)

合约部署者在主网启动时部署该合约。
USDT部署者是usdt.bos;
BTC部署者是btc.bos;
ETH部署者是eth.bos;

承兑商由BP多签创建，每种币可以有多个承兑商。但在BOS链上的币名不能相同。
举例：BTC在BOS链上的相应的币名可以为BTC、BBTC、BOSBTC等。
一个承兑商可以承兑多种币，一种币只能用一个承兑商。

每个承兑商相应币种的承兑人有且只有一个。
承兑人的功能：（a）给用户转账（b）接收用户转账

每个承兑商的审核员至少一个、可以多个。
审核员的功能：（a）同意或拒绝大额提币 （b）提币失败时, 将币退回用户账户。

充币：在BOS链上，承兑商的承兑人给普通用户转账

提币：在BOS链上，普通用户给承兑商的承兑人转账

合约部署账号:  usdt.bos btc.bos  eth.bos
承兑商(issuer)账号:   boshuobipool
承兑商相应币种承兑人（acceptor）的账号:
	BTC:  btcbtcbtcbtc
	ETH:  ethethetheth
    USDT: usdtusdtusdt

承兑商审核员（auditor）账号： huobiauditor, auditorhuobi
承兑商合作方(partner)账号:  huobipartner, partnerhuobi
```

# 第二部分：接口设计

### 一、 create【BP 多签】

功能： 创建承兑商及币种。
承兑商账号不能是合约部署账号，如 btc.bos。
权限:   BP 多签

```
symbol  sym  //币名。如BTC、ETH、USDT   包含精度
name    issuer     //承兑商账户, 如boshuobipool
name address_style  //币种的地址类型: bitcoin、ethereum、tether
```

大致流程：

1. 创建币名、承兑商、地址类型、币的机制。
2. 填充symbols表的sym字段
3. 填充infos表的issuer、address_style字段

同时初始化：

1. supply：BTC   0.00000000        ETH    0.00000000   USDT    0.00000000
2. active: true

注意相应币种的精度：BTC  8    ETH 8  USDT    8

```
cleos push action btc.bos create '["8,BTC", "boshuobipool",  "bitcoin"]' -p btc.bos
cleos push action eth.bos create '["8,ETH", "boshuobipool",  "ethereum"]' -p eth.bos
cleos push action usdt.bos create '["8,USDT", "boshuobipool",  "tether"]' -p usdt.bos
```

### 二、setissuer【BP多签】

功能： 设置承兑商。 承兑商账号不能是合约部署账号，如btc.bos。
权限:   BP多签
版本：1和2
机制：预发行制和严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
name    issuer     //承兑商账户, 如boshuobipool
```

大致流程：

- 根据sym_code，修改infos表的issuer字段。

```
cleos push action btc.bos setissuer '["BTC", "boshuobipool"]' -p btc.bos
cleos push action eth.bos setissuer '["ETH", "boshuobipool"]' -p eth.bos
cleos push action usdt.bos setissuer '["USDT", "boshuobipool"]' -p usdt.bos
```

### 三、setauditor 

功能：承兑商添加或者删除自己的审核员。可以设置多个审核员。
权限：承兑商
机制：预发行制和严格锚定制
参数：

```
symbol_code sym_code //币名
string  actn          //动作：添加或删除     字符串为 add 或 remove
name    auditor     //承兑商的某个审核员账户
```

大致流程：

1. 校验权限。
2. auditor账号必须是合法的BOS账号。必须真实存在。
3. auditor账号不能是角色账号。
4. 需要判断账号是否已经绑定了地址、是否有余额，如果满足任意一条，则报错。
5. 根据sym_code，在auditors表中增加或删除一条记录。
6. action为"add"时，增加记录；action为"remove"时，删除记录。

```
cleos push action btc.bos setauditor '["BTC","add","huobiauditor"]' -p boshuobipool
cleos push action btc.bos setauditor '["BTC","remove","huobiauditor"]' -p boshuobipool
```

### 四、setgatherer

功能：承兑商添加或者删除自己的收费员。有且只有一个收费员。
权限：承兑商
机制：严格锚定制
参数：

```
symbol_code sym_code //币名
name    gatherer     //承兑商的收费员账户
```

大致流程：

1. 校验权限。
2. gatherer账号必须是合法的BOS账号。必须真实存在。
3. gatherer账号不能是角色账号。
4. 需要判断账号是否已经绑定了地址、是否有余额，如果满足任意一条，则报错。
5. 根据sym_code，在gatherers表中增加或修改一条记录。

```
cleos push action btc.bos setgatherer '["BTC","huobigatherer"]' -p boshuobipool
cleos push action btc.bos setgatherer '["BTC","huobigatherer"]' -p boshuobipool
```

### 五、setteller

功能：承兑商添加或者修改相应币种的出纳员。必须设置且只能设置一个出纳员。
权限：承兑商
机制：严格锚定制
参数：

```
symbol_code sym_code //币名
name    teller     //承兑商的出纳员账户
```

大致流程：

1. 校验权限。
2. teller账号必须是合法的BOS账号。必须真实存在。
3. teller账号不能是角色账号。
4. 需要判断新的teller是否绑定了地址，若有，则报错。
5. 需要判断原有的teller是否还有余额，如果有，则报错。
6. 根据sym_code，增加或修改tellers表的teller字段。

```
cleos push action btc.bos setteller '["BTC","btcbtcbtcbtc"]' -p boshuobipool
cleos push action eth.bos setteller '["ETH","ethethetheth"]' -p boshuobipool
cleos push action usdt.bos setteller '["USDT","usdtusdtusdt"]' -p boshuobipool
```

### 六、setmanager

功能：承兑商添加或者删除自己的管理员。必须设置且只能设置一个管理员。
权限：承兑商
机制：严格锚定制
参数：

```
symbol_code sym_code //币名
name    manager     //承兑商的管理员账户
```

大致流程：

1. 校验权限。
2. manager账号必须是合法的BOS账号。必须真实存在。
3. manager账号不能是其他角色账号。
4. 需要判断账号是否已经绑定了地址、是否有余额，如果满足任意一条，则报错。
5. 根据sym_code，在managers表中增加或删除一条记录。

```
cleos push action btc.bos setmanager '["BTC","huobimanager"]' -p boshuobipool
cleos push action btc.bos setmanager '["BTC","huobimanager"]' -p boshuobipool
```

### 七、setbrakeman

功能：承兑商添加或者删除自己的制动员。必须设置且只能设置一个制动员。
权限：承兑商
机制：严格锚定制
参数：

```
symbol_code sym_code //币名
name    brakeman     //承兑商的制动员账户
```

大致流程：

1. 校验权限。
2. brakeman账号必须是合法的BOS账号。必须真实存在。
3. brakeman账号不能其他角色账号。
4. 需要判断账号是否已经绑定了地址、是否有余额，如果满足任意一条，则报错。
5. 根据sym_code，在brakemans表中增加或删除一条记录。

```
cleos push action btc.bos setbrakeman '["BTC","hbhbbrakeman"]' -p boshuobipool
cleos push action btc.bos setbrakeman '["BTC","hbhbbrakeman"]' -p boshuobipool
```

### 八、 setlimit

功能：承兑商设置限额。
包括：单次最大换币金额、单次最小换币金额、单日累计换币限额、单日累计换币次数、两次换币间隔
权限：管理员
机制：严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
asset   maximum_limit  //单次最大换币金额。每个账号每次换币金额大于此数, 承兑商不换币。默认值为：BTC 1   ETH 1  USDT 1
asset   minimum_limit  //单次最小换币金额。每个账号每次换币金额小于此数，承兑商不换币。默认值为：BTC 0.00005  ETH 0.00005 USDT 0.00005
asset   total_limit  //单日累计换币限额。每个账号每日换币总额大于此数，承兑商不换币。默认值为：BTC 10 ETH 10  USDT 10
uint64_t frequency_limit   //单日累计换币次数。 每个账号每日换币次数大于此数，则承兑商不换币。默认值为：BTC 3 ETH 3  USDT 3
uint64_t interval_limit //两次换币的间隔秒数。每个账号两次换币间隔秒数小于此数，承兑商不换币。默认值为：
```

大致流程：

1. 校验权限。
2. 判断所有的asset是否与sym_code为同一种币，若不是，则报错。
3. 根据sym_code，增加或者修改limits表的maximum_limit、minimum_limit、total_limit、frequency_limit、interval_limit字段。若为增加，则其他字段为0。

```
cleos push action btc.bos setlimit '["BTC", "1.00000000 BTC","0.00005000 BTC","10.00000000 BTC","3","300"]' -p huobimanager
cleos push action eth.bos setlimit '["ETH","1.00000000 ETH","0.00005000 ETH","10.00000000 ETH","3","300"]' -p huobimanager
cleos push action usdt.bos setlimit '["USDT","1.00000000 USDT","0.00005000 USDT","10.00000000 USDT","3","300"]' -p huobimanager
```

### 九、setfee

功能：承兑商设置服务费。提币/销币时收取，充币/铸币时不收取。按提币/销币金额收取相应费率的手续费。有最低收费。
权限：管理员
机制：严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
double  service_fee_rate     //服务费率   默认0.001 （千分之一）
asset   min_service_fee     //服务费最低收费  默认0.0005
asset   miner_fee  //矿工费  默认 BTC 0.00004 ETH 0.00004
```

大致流程：

1. 校验权限。
2. 判断所有的asset是否与sym_code为同一种币，若不是，则报错。
3. 根据sym_code，增加或修改fees表的service_fee_rate、min_service_fee、miner_fee字段。若为增加，则其他字段为0。

```
cleos push action btc.bos setfee '["BTC", "0.001","0.00005000 BTC", "0.00004000 BTC"]' -p huobimanager
cleos push action eth.bos setfee 
'["ETH","0.001","0.00005000 ETH", "0.00004000 ETH"]' -p huobimanager
cleos push action usdt.bos setfee '["USDT","0.001","0.00005000 USDT", "0.00004000 USDT"]' -p huobimanager
```

### 十、setvip

功能：承兑商添加或者修改相应币种的某个用户为vip用户。
权限：管理员
机制：严格锚定制
参数：

```
symbol_code sym_code //币名
string  actn          //动作：添加或删除     字符串为 add 或 remove
name    vip     //vip用户账户
```

大致流程：

1. 校验权限。
2. 根据sym_code确定scope, 在vips表中增加或删除一条记录。
3. 补充：vips表初始化后，同时也需要初始化vipfees和viplimits表。
4. action为"add"时，增加记录；action为"remove"时，删除记录。
5. vip账号必须是合法的BOS账号。必须真实存在。
6. vip账号不能是角色账号。
7. **需要验证该vip账户是否已经被设置成vip账户，若已经设置，则禁用**

```
cleos push action btc.bos setvip '["BTC", "add", "huobihuaping"]' -p huobimanager
cleos push action eth.bos setvip '["ETH", "add", "huobihuaping"]' -p huobimanager
cleos push action usdt.bos setvip '["USDT","add", "huobihuaping"]' -p huobimanager
```

### 十一、setviplimit

功能：承兑商设置单个用户的单次最大换币金额、单次最小换币金额、单日累计换币限额、单日累计换币次数、两次换币的间隔
权限：管理员
机制：严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
name vip //VIP用户账号
asset   maximum_limit  //单次最大换币金额。每个账号每次换币金额大于此数, 承兑商不换币。默认值为：BTC 1   ETH 1  USDT 1
asset   minimum_limit  //单次最小换币金额。每个账号每次换币金额小于此数，承兑商不换币。默认值为：BTC 0.00005  ETH 0.00005 USDT 0.00005
asset   total_limit  //单日累计换币限额。每个账号每日换币总额大于此数，承兑商不换币。默认值为：BTC 10 ETH 10  USDT 10
uint64_t frequency_limit   //单日累计换币次数。 每个账号每日换币次数大于此数，则承兑商不换币。默认值为：BTC 3 ETH 3  USDT 3
uint64_t interval_limit //两次换币的间隔秒数。每个账号两次换币间隔秒数小于次数，承兑商不换币。默认值为：300
```

大致流程：

1. 校验权限。
2. 判断所有的asset是否与sym_code为同一种币。
3. 根据sym_code和vip的账户名，修改vips表的maximum_limit、minimum_limit、total_limit、frequency_limit、interval_limit字段

```
cleos push action btc.bos setviplimit '["BTC", "huobihuaping", "1.00000000 BTC","0.00005000 BTC","10.00000000 BTC","3","300"]' -p huobimanager
cleos push action eth.bos setviplimit '["ETH","huobihuaping", "1.00000000 ETH","0.00005000 ETH","10.00000000 ETH","3","300"]' -p huobimanager
cleos push action usdt.bos setviplimit '["USDT","huobihuaping", "1.00000000 USDT","0.00005000 USDT","10.00000000 USDT","3","300"]' -p huobimanager
```

### 十二、setvipfee

功能：承兑商设置单个用户的服务费。提币/销币时收取，充币/铸币时不收取。按提币/销币金额收取相应费率的手续费。
权限：管理员
机制：严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
name vip //VIP用户账号
double  service_fee_rate     //服务费率   默认0.001 （千分之一）
asset   min_service_fee     //服务费最低收费  默认0.0005
asset   miner_fee  //矿工费  默认 BTC 0.00004 ETH 0.00004
```

大致流程：

1. 校验权限。

2. 判断所有的asset是否与sym_code为同一种币，若不是，则报错。

3. 根据sym_code，以及 vip的账户名,  修改vips表的service_fee_rate、min_service_fee、miner_fee字段。

```
cleos push action btc.bos setvipfee '["BTC", "huobihuaping","0.001","0.00005000 BTC", "0.00004000 BTC"]' -p huobimanager
cleos push action eth.bos setvipfee 
'["ETH","huobihuaping", "0.001","0.00005000 ETH", "0.00004000 ETH"]' -p huobimanager
cleos push action usdt.bos setvipfee '["USDT","huobihuaping", "0.001","0.00005000 USDT", "0.00004000 USDT"]' -p huobimanager
```

### 十三、setcheck

功能：承兑商设置资金出入是否需要审核。
权限：管理员
机制：严格锚定制
参数：

```
symbol_code  sym_code  //币名。如BTC、ETH、USDT
bool in_check  //相对于BOS链的资金流入是否需要审核。false：无需审核；true：需要审核。默认为false。
bool out_check //相对于BOS链的资金流出是否需要审核。false：无需审核；true：需要审核。 默认为false。
```

大致流程：

1. 校验权限。
2. 根据sym_code， 修改checks表的in_check、out_check字段。

```
cleos push action btc.bos setcheck '["BTC", true,false]' -p huobimanager
cleos push action eth.bos setcheck '["ETH",true,true]' -p huobimanager
cleos push action usdt.bos setcheck '["USDT",false,true]' -p huobimanager
```

### 十四、resetaddress

功能：承兑商重置地址。
权限：管理员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
name to   //需要分配地址的账号
```

大致流程：

1. 校验权限。
2. 校验to是否为合法的BOS账号。此账号必须在BOS上真实存在。不能是除gatherer外的角色账号。
3. 若to不在addrs表，则报错。
4. 根据to, 读取addrs表，获得该用户已分配地址，若为空，则报错。
5. 根据sym_code和to，修改addrs表中一条记录。将address字段置为空， state字段置为owner.value。

```
cleos push action btc.bos resetaddress '[ "BTC", "xiaomingming" ]' -p huobimanager
cleos push action eth.bos resetaddress '[ "ETH", "huobihuaping" ]' -p huobimanager
cleos push action usdt.bos resetaddress '[ "USDT", "huobihuaping" ]' -p huobimanager
```

### 十五、precast

功能：承兑商预铸币。用户从其他链向BOS链换币时，在BOS链上，承兑商给普通用户预转账。注意：此时并未真正转账，需要审核员审核。用于需要审核的场景。由承兑商保证1：1兑换。例如：承兑商在BTC主网的所有地址中有1000个BTC，则在BOS链也存在1000个BTC。

权限：出纳员。
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
string to_address //BTC地址、ETH地址、USDT地址
name to_account //普通用户账号。
string remote_trx_id //BTC链、ETH链、USDT链的交易号
asset quantity       //需要发行的相应币种的数额
uint64_t index //序号
string memo        //备注
```

大致流程： 

1. 根据sym_code，查询checks表，校验审核配置，配置不对则报错。此函数只用于“资金流入需要审核”的场景。
2. 判断是否已经锁定，如果已锁定，则报错并提示。
3. 校验权限。 
4. 校验to_account账号的合法性。
5. 此账号必须在BOS上真实存在。不能是角色账户。 
6. quantity必须大于0 
7. BTC 精度为8 ETH精度为8 USDT精度为8 
8. memo可以为空 
9. 根据quantity所携带的symbol信息确定scope 
10. create_time为当前时间。 msg填充memo字段。 
11. 铸币前, 查表casts, 若表中已经有相同的铸币hash+index，则报错。 
12. 铸币前, 判断to_address在链上通过assignaddr绑定的account，是否与参数to_account值一致，若不一致，则报错。
13. 向casts表插入一条数据，need_check置为true，enable字段置为false。

```
cleos push action btc.bos precast '["BTC", "3JQSigWTCHyBLRD979JWgEtWP5YiiFwcQB", "huobihuaping", "fe2f90783080925ef63b9369f3e00df6860d921806655543f582e869e4652619",  "10000.00100000 BTC", "1","memo"]' -p btcbtcbtcbtc

cleos push action eth.bos precast '["ETH", "0xe53d7a25f9b1769d0a9b6ae674155b6be886252d", "huobihuaping", "5b641b15168df9ce49295c694ed9e77608b0708bc60238b100eb483ec481ead2",  "1990.00100091 ETH", "1","memo"]' -p ethethetheth

cleos push action usdt.bos precast '["USDT", "3JQSigWTCHyBLRD979JWgEtWP5YiiFwcQB", "huobihuaping", "fe2f90783080925ef63b9369f3e00df6860d921806655543f582e869e4652619",  "10000.00100000 USDT", "1","memo"]' -p usdtusdtusdt
```

### 十六、assignaddr

功能：承兑商给账户分配地址。    
权限：出纳员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
name to   //需要分配地址的账号
string address  //相应币种地址，如BTC、ETH、USDT
```

大致流程：

1. 校验权限。
2. 校验address是否为相应币种的合法地址。
3. 校验to是否为合法的BOS账号。此账号必须在BOS上真实存在。不能是除gatherer外的角色账号。 
4. 根据address，查询addrs表和records表，若存在，则报错。
5. 根据to，查询addrs表，若不存在，则报错；若address字段不为空，则报错。
6. 根据sym_code和to，在**addrs**表中修改一条记录，修改address字段， state字段置为0，assign_time为当前时间

```
cleos push action btc.bos assignaddr '["BTC", "huobihuaping", "1F7XiwJTfZEpFDGikNC71U6MBBeLsrsDd6"]' -p btcbtcbtcbtc
cleos push action eth.bos assignaddr '["ETH", "huobihuaping", "0x8232Fe62877517895ee43FFFB0B079E69293d5e2"]' -p ethethetheth
cleos push action usdt.bos assignaddr '["USDT", "huobihuaping", "1F7XiwJTfZEpFDGikNC71U6MBBeLsrsDd6"]' -p usdtusdtusdt
```

### 十七、confirmback

功能： 从BOS链向其他链换币，成功时，承兑商反馈信息。
权限：出纳员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH
string remote_trx_id  //其他链上提币的交易号
uint64_t id           //melts表记录中的主键id
uint64_t remote_index //序号  表明其他链上的一笔交易对应的vin/vout index
string memo  //备注
```

大致流程：

1. 判断是否已经锁定，如果已锁定，则报错并提示。
2. 校验权限。
3. 根据sym_code确定scope。
4. 根据主键id查询melts表记录，填充melts表中的remote_trx_id和remote_index,memo可以为空。
5. 将state置为2.

### 十八、denyback

功能： 从BOS链向其他链换币，失败时，承兑商反馈信息。
权限：出纳员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH
uint64_t id //melts表记录中的主键id
string memo  //备注
```

大致流程：

1. 判断是否已经锁定，如果已锁定，则报错并提示。
2. 校验权限。
3. 根据sym_code确定scope。
4. 执行条件应为need_check=true && enable=true && state=0 或者 need_check=false && state=0（即melt需要审核且审核通过，或者melt无须审核的情况）
5. memo可以为空。
6. 将state置为5.
7. 根据主键id查询melts表记录，查出melts表的amount，填充melts表中的state
8. 根据sym_code，增加infos表的supply字段的值，值为amount。
9. 增加accounts表的balance值，值为amount。**以内联action的方式调用retreat**

### 十九、agreecast

功能：审核员放行铸币。用于需要审核的场景。
权限：审核员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
string to_address //BTC地址、ETH地址、USDT地址
name to_account //普通用户账号。
name auditor //审核员账号。
string remote_trx_id //BTC链、ETH链、USDT链的交易号
asset quantity       //需要发行的相应币种的数额
uint64_t index //序号 主链交易的vout index
string memo        //备注
```

大致流程： 

1. 根据sym_code，查询checks表，校验审核配置，配置不对则报错。此函数只用于“资金流入需要审核”的场景。
2. 判断是否已经锁定，如果已锁定，则报错并提示。
3. 校验权限。 
4. 校验to_account账号的合法性。此账号必须在BOS上真实存在。不能是角色账户。 
5. quantity必须大于0 
6. BTC 精度为8 ETH精度为8 USDT精度为8 
7. memo可以为空 
8. 根据sym_code确定scope 。判断sym_code和quantity所携带的symbol信息是否相同。
9. 查表casts, 若表中没有相同的铸币hash+index，则报错。 
10. 判断to_address在链上通过assignaddr绑定的account，是否与参数to_account值一致，若不一致，则报错。
11. 增加to_account的余额 
12. 增加infos表中supply的值
13. 修改casts表之前，检查state是否为0，若否则报错，不能操作已完成cast；agreecast成功后，将state改为2
14. 修改casts表，生成交易号填充casts的trx_id字段，update_time为当前时间。 msg填充memo字段。
15. 填充casts的index字段，值默认为0
16. 当need_check为true且enable字段为false时，将enable字段置为true。
17. 记录审核员账户。

### 二十、refusecast

功能：审核员拒绝铸币。用于需要审核的场景。
权限：审核员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
string to_address //BTC地址、ETH地址、USDT地址
name to_account //普通用户账号。
name auditor //审核员账号。
string remote_trx_id //BTC链、ETH链、USDT链的交易号
asset quantity       //需要发行的相应币种的数额
uint64_t index //序号
string memo        //备注
```

大致流程： 

1. 根据sym_code，查询checks表，校验审核配置，配置不对则报错。此函数只用于“资金流入需要审核”的场景。
2. 判断是否已经锁定，如果已锁定，则报错并提示。
3. 校验权限。 
4. 校验to_account账号的合法性。
5. 此账号必须在BOS上真实存在。不能是角色账户。 
6. quantity必须大于0 
7. BTC 精度为8 ETH精度为8 USDT精度为8 
8. memo可以为空 
9. 根据quantity所携带的symbol信息确定scope 
10. update_time为当前时间。 msg填充memo字段。 
11. 查表casts, 若表中没有相同的铸币hash+index，则报错。 
12. 判断to_address在链上通过assignaddr绑定的account，是否与参数to_account值一致，若不一致，则报错。
13. 修改casts表之前，检查state是否为0，若否则报错，不能操作已完成cast；refusecast成功后，将state改为5
14. 记录审核员账户。
15. 获取本次交易的trx_id和index，填充casts表的trx_id和index字段，index值默认为0

### 二十一、lockall

功能： 承兑商锁定服务。此时所有的提币、充币、转账都无法进行。
权限：制动员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
name  brakeman //制动员账号
```

大致流程：

1. 校验权限。
2. 根据sym_code， 修改infos的active字段为false。

```
cleos push action btc.bos lockall '["BTC","hbhbbrakeman"]' -p hbhbbrakeman
cleos push action eth.bos lockall '["ETH","hbhbbrakeman"]' -p hbhbbrakeman
cleos push action usdt.bos lockall '["USDT","hbhbbrakeman"]' -p hbhbbrakeman
```

### 二十二、unlockall

功能： 承兑商恢复服务。此时所有的提币、充币、转账都恢复正常。
权限：制动员
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
name brakeman //制动员账号
```

大致流程：

1. 校验权限。
2. 根据sym_code， 修改infos的active字段为true。

```
cleos push action btc.bos unlockall '["BTC","hbhbbrakeman"]' -p hbhbbrakeman
cleos push action eth.bos unlockall '["ETH","hbhbbrakeman"]' -p hbhbbrakeman
cleos push action usdt.bos unlockall '["USDT","hbhbbrakeman"]' -p hbhbbrakeman
```

### 二十三、 melt

功能：承兑商销币。普通用户从BOS链向其他链换币时，用户将资产转给承兑商的出纳员。 用于无需审核的场景。由承兑商保证1：1兑换。例如：承兑商在BTC主网的所有地址中有1000个BTC，则在BOS链也存在1000个BTC。  
权限：普通用户
机制：严格锚定制
参数：

```
name from_account //普通用户账号。
string to_address //提币的目的地。若为BTC或ETH或USDT，则该字段填入BTC或ETH或USDT的地址。需要校验BTC地址、ETH地址、USDT地址的合法性。
asset quantity       //需要销毁的相应币种的总额
string memo        //备注
```

大致流程： 

1. 根据sym_code，查询checks表，校验审核配置，配置不对则报错。此函数只用于“资金流出无需审核”的场景。
2. 判断是否已经锁定，如果已锁定，则报错并提示。
3. 校验权限。 在具体版本中进行校验
4. 校验from_account是否为合法的BOS账号。此账号必须在BOS上真实存在。 不能为除gatherer外的角色账号。
5. 校验to_address是否为合法的BTC地址、ETH地址、USDT的地址。 
6. quantity必须大于0 
7. 补充：from_account账户余额必须大于quantity
8. BTC 精度为8 ETH精度为8 USDT精度为8 
9. 根据quantity所携带的symbol信息确定scope 
10. 若符合下述“禁止提币”的条件，则报错。其中：若为vip用户，则判断该用户的限制条件；若不是vip用户，则使用通用的限制条件。
11. 计算费用。计算方式为：max(总额乘以费率， 最小收费)
12. 其中费率和最小收费的取值为：如果为VIP用户，则为VIP用户的费率和最小收费；否则，为通用的费率和最小收费。
13. 将费用转到收费人账号，即收费人账号的余额增加。以内联action的方式调用pay。
14. 减小from_account的余额 ，数值为总额减去费用；减小infos表中的supply的值，数值为总额减去费用。以内联action的方式调用ruin。
15. 在melts添加一条记录，need_check置为false，enable字段置为false。
16. 将melts中的记录主键id，传参出去，以便区块链解析程序捕获。以内联action的方式调用notifymelt。

```
cleos push action eth.bos melt '["huobihuaping", "0xe53d7a25f9b1769d0a9b6ae674155b6be886252d", "190.00100091 ETH","1","memo"]' -p huobihuaping

cleos push action btc.bos melt '["huobihuaping", "3JQSigWTCHyBLRD979JWgEtWP5YiiFwcQB", "100.00100000 BTC","1","memo"]' -p huobihuaping

cleos push action usdt.bos melt '["huobihuaping", "3JQSigWTCHyBLRD979JWgEtWP5YiiFwcQB", "100.00100000 USDT","1","memo"]' -p huobihuaping

cleos push action usdt.bos melt '["huobihuaping", "0xe53d7a25f9b1769d0a9b6ae674155b6be886252d", "190.00100091 USDT","1","memo"]' -p huobihuaping
```

### 二十四、applyaddr

功能：用户申请分配地址。    
权限：普通用户
机制：严格锚定制
参数：

```
symbol_code sym_code  //币名。如BTC、ETH、USDT
name to   //需要分配地址的账号
```

大致流程：

1. 校验权限。

2. 校验to是否为合法的BOS账号。此账号必须在BOS上真实存在。不能是除gatherer外的角色账号。

3. 若to在addrs表，则报错。

4. 根据sym_code，在addrs表中增加一条记录。

5. 此时owner字段置为to，address字段为空， state字段为owner.value

6. create_time为当前时间，**assign_time为空**

### 二十五、 ruin

功能：普通用户毁掉代币。
权限：普通用户
机制：严格锚定制
参数：

```
asset   quantity  //需要毁掉的资产总数
name user
```

大致流程：

1. 判断是否已经锁定，如果已锁定，则报错并提示。
2. 校验权限。
3. quantity必须大于0。 注意精度。
4. 根据quantity携带symbol确定scope。
5. 查找accounts表，减小普通用户的balance值。

```
cleos push action btc.bos ruin '[ "0.90000000 BTC", "huobihuaping" ]' -p huobihuaping
cleos push action eth.bos ruin '[ "0.90000000 ETH", "huobihuaping" ]' -p huobihuaping
cleos push action usdt.bos ruin '[ "0.90000000 USDT", "huobihuaping" ]' -p huobihuaping
```

### 二十六、pay

功能：普通用户给收费员转账。
权限：普通用户
机制：严格锚定制
参数：

```
asset   quantity  //需要转账的资产总数
name  user
```

大致流程：

1. 判断是否已经锁定，如果已锁定，则报错并提示。
2. 校验权限。
3. quantity必须大于0。 注意精度。
4. 根据quantity携带symbol确定scope。
5. 查找accounts表，减小普通用户的balance值，增加收费员的balance值。

```
cleos push action btc.bos pay '[ "0.90000000 BTC", "huobihuaping" ]' -p huobihuaping
cleos push action eth.bos pay '[ "0.90000000 ETH", "huobihuaping" ]' -p huobihuaping
cleos push action usdt.bos pay '[ "0.90000000 USDT", "huobihuaping" ]' -p huobihuaping
```

### 二十七、retreat

功能：退回代币
权限：出纳员
机制：严格锚定制
参数：

```
name    to //转入账号
asset   quantity  //需要生成的资产总数
```

大致流程：

1. 判断是否已经锁定，如果已锁定，则报错并提示。
2. 校验权限。
3. quantity必须大于0。 注意精度。
4. 根据quantity携带symbol确定scope。
5. 查找accounts表，增加普通用户的balance值。

### 二十七、notifymelt

功能：传递销币记录id。
权限：普通用户
机制：严格锚定制
参数：
```
uint64_t id  //序号。melts表记录的主键id。
```

大致流程：
函数不需要实现任何逻辑，只需要传参melts表记录的主键id。

# 第三部分：表结构设计

### 表 symbols

    介绍：币种表

每个币一条记录。
字段：

```
symbol  sym  //币种，如ETH、BTC等。主键
```

### 表 checks

介绍：币种审核表, 每个币一条记录。
字段：

```
symbol  sym   //币种，如ETH、BTC、USDT等。主键
bool in_check   //相对于BOS链的资金流入是否需要审核。false：无需审核；true：需要审核。默认为false。
bool out_check  //相对于BOS链的资金流出是否需要审核。false：无需审核；true：需要审核。 默认为false。
```

### 表 addrs [币种]

    介绍：账号地址映射表

每个账号一条记录。
scope: 币种 如 BTC、ETH、BTCZB、ETHZB
字段：

```
name  owner  //普通用户账号。   主键  
string  address //其他链的地址。第二索引
uint64_t  state //状态值。未分配地址时为非0，取值为owner.value；分配地址后为0。 第三索引。
time_point_sec    create_time  //申请地址时间
time_point_sec   assign_time   //地址映射时间
```

```
cleos get table btc.bos BTC addrs
cleos get table eth.bos ETH addrs
```

### 表 records [币种]

介绍：账号地址映射历史记录表

每个地址一条记录。

scope:  币种 如BTC、ETH、BTCZB、ETHZB 

字段：

```
name  owner  //普通用户账号。     第二索引。
string  address //其他链的地址。主键
time_point_sec   reset_time   //重置时间
```

### 表 statistics [币种]

介绍：用户的提币信息统计

scope: 币种 如BTC、ETH

字段：

```
name  owner  //普通用户账号。   主键    
time_point_sec   last_time  //上次提币时间 
frequency // 24h之内用户的提币次数上限。如果本次提币距离上次提币超过24h，则重置次数
total  // 24h之内用户的提币总额上限。如果本次提币距离上次提币超过24h，则重置总额
time_point_sec       update_time    //统计时间
```

```
cleos get table btc.bos BTC statistics
cleos get table eth.bos ETH statistics
```

### 表 accounts [用户]

介绍：账号信息表, 每个账号一条记录
scope: 用户名
字段：

```
asset    balance //账号拥有的该币种的余额。主键。
```

```
cless get table btc.bos huobihuaping accounts
cleos get table eth.bos huobiliqiang accounts
```

### 表 melts [币种]

介绍：用户提币记录

scope: 币种 如BTC、ETH、USDT 

字段：

```
uint64_t id  //序号。主键。
transaction_id_type   trx_id  //BOS链上的交易号。第二索引
name        from  //普通用户账号
string      to  //BTC地址、ETH地址、USDT地址
asset                total //本次提币总额
asset                amount //本次提币金额
asset                fee //本次提币费用
uint64_t    state //状态值。提币成功为2；提币退回为3；提币回滚为5。 第三索引。
bool                 need_check //是否需要审核。无需审核为false;需要审核为true；默认为false。
bool                 enable //是否放行。审核员拒绝时，置为false;审核员同意时，置为true；其他情况默认置为false。
name            auditor //审核员账号。审核这笔提币的审核员账号。默认为空。
uint64_t index //序号
string      remote_trx_id //其他链的交易号
uint64_t remote_index //表明其他链上的一笔交易对应的vin/vout index
string      msg //备注信息
time_point_sec       create_time //申请提币时间
time_point_sec       update_time //提币成功、提币回滚、提币退回的时间
```

```
cleos get table btc.bos BTC melts
cleos get table eth.bos ETH melts
cleos get table usdt.bos USDT melts
```

注：total=amount+fee

### 表 casts [币种]

介绍：用户充币记录

scope: 币种 如BTC、ETH、USDT 

字段：

```
uint64_t id  //序号。主键。
transaction_id_type  trx_id  //BOS链上的交易号。第二索引
name               to_account  //普通用户账号
string               to_address  //BTC地址、ETH地址、USDT地址
asset                quantity //充币总额
uint64_t             state //状态值。第三索引。预留。初始为0。
                           // 补充：初始化为0，铸币成功为2，拒绝铸币为5
bool                 need_check //是否需要审核。无需审核为false;需要审核为true；默认为false。
bool                 enable //是否放行。审核员拒绝时，置为false;审核员同意时，置为true；其他情况默认置为false。
name            auditor //审核员账号。审核这笔提币的审核员账号。默认为空。
uint64_t index //序号
string               remote_trx_id //其他链的交易号
uint64_t remote_index //表明其他链上的一笔交易对应的vin/vout index
string               msg //备注信息
time_point_sec       create_time //充币时间
time_point_sec       update_time //更新时间。预留。先设为与create_time相同。
```

```
cleos get table btc.bos BTC casts
cleos get table eth.bos ETH casts
cleos get table usdt.bos USDT casts
```

### 表 vips [币种]

介绍：VIP用户列表

scope: 币种 如BTC、ETH

字段：

```
name  vip  //用户账号。   主键    
time_point_sec    create_time    //时间
```

```
cleos get table btc.bos BTC vips
cleos get table eth.bos ETH vips
```

### 表 viplimits  [用户]

介绍：VIP账号限制表, 每个账号一条记录

scope: 币种 如BTC、ETH

字段：

```
name  owner  //vip用户账号。   主键 
asset    maximum_limit //单次最大换币金额
asset    minimum_limit //单次最小换币金额
asset    total_limit //单日累计换币总额
uint64_t frequency_limit   //单日累计换币次数
uint64_t interval_limit  //两次换币间隔秒数
uint64_t reset_limit //两次重置绑定地址的秒数
```

```
cless get table btc.bos BTC viplimits
cleos get table eth.bos ETH viplimits
```

### 表 vipfees  [用户]

介绍：VIP账号费用表, 每个账号一条记录

scope: 币种 如BTC、ETH

字段：

    name  owner  //vip用户账号。   主键 
    double   service_fee_rate //服务费率
    asset    min_service_fee //服务费最低收费
    asset    miner_fee //矿工费

```
cless get table btc.bos BTC vipfees
cleos get table eth.bos ETH vipfees
```

### 表 infos [币种]

介绍：币种信息表

scope: 币种 如BTC、ETH、USDT

字段：

    asset  supply //该币种发行量。  币种名为主键。 链上所有账号的余额之和等于发行量。
    name   issuer //承兑商账号  非空
    name   address_style; //币的类型 如：bitcoin、ethereum、tether
    bool   active //是否正常提供服务

```
cleos get table btc.bos BTC infos
cleos get table eth.bos ETH infos
cleos get table usdt.bos USDT infos
```

### 表 limits [币种]

介绍：币种限制表

scope: 币种 如BTC、ETH、USDT

字段：

```
asset    max_limit //单次最大换币金额
asset    min_limit //单次最小换币金额
asset    total_limit //单日累计换币总额
uint64_t frequency_limit   //单日累计换币次数
uint64_t interval_limit  //两次换币间隔秒数
uint64_t reset_limit //两次重置绑定地址的秒数
```

### 表 fees [币种]

介绍：币种费用表

scope: 币种 如BTC、ETH、USDT

字段：

```
double   service_fee_rate //服务费率
asset    min_service_fee //服务费最低收费
asset    miner_fee //矿工费
```

### 表 managers 

介绍：承兑商的管理员, 每个管理员一条记录。

scope: 币种 如BTC、ETH

字段：

```
name  manager  //管理员账号。主键
```

```
cleos get table btc.bos BTC managers
cleos get table eth.bos ETH managers
```

### 表 tellers 

介绍：承兑商的出纳员, 每个出纳员一条记录。

scope: 币种 如BTC、ETH、USDT

字段：

```
name  teller  //账号。主键
```

```
cleos get table btc.bos BTC tellers
cleos get table eth.bos ETH tellers
```

### 表 auditors 

介绍：承兑商的审核员, 每个审核员一条记录。

scope: 币种 如BTC、ETH

字段：

```
name  auditor  //审核员账号。主键
```

```
cleos get table btc.bos BTC auditors
cleos get table eth.bos ETH auditors
```

### 表 gathers 

介绍：承兑商的收费员, 每个收费员一条记录。

scope: 币种 如BTC、ETH、USDT

字段：

```
name  gatherer  //收费员账号。主键
```

```
cleos get table btc.bos BTC gathers
cleos get table eth.bos ETH gathers
```

### 表 brakemans 

介绍：承兑商的制动员, 每个制动员一条记录。

scope: 币种 如BTC、ETH、USDT

字段：

```
name  brakeman  //制动员账号。主键
```

```
cleos get table btc.bos BTC brakemans
cleos get table eth.bos ETH brakemans
```