# TableManager
一款基于Qt的表格管理软件，使用QXlsx对xlsx表格数据进行操作.

表格模板为ShipTabel.xlsx，用户信息存储在info.json中。

该项目目前是在远程文件服务器里读取文件下载到本地，如果您只需在本地读取，只需删除downloadTable()和downloadUserInfo()，并在config.json中设置table_path和user_info_path为对应的本地路径即可。

![image](https://github.com/yzhperseverance/TableManager/assets/79842719/04f29624-d1fd-4a89-adf3-c7765829a80d)

![image](https://github.com/yzhperseverance/TableManager/assets/79842719/d37a8710-350d-4bb6-a7fc-092840d7ba08)

