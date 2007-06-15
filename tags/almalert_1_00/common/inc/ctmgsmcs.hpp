/*
    ctmgsmcs.hpp
    Copyright (C) 2005-2006 zg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __CTMGSMCS_HPP__
#define __CTMGSMCS_HPP__

#define IPC_GSM_CTM_EXT (10000)

enum
{
  EGsmPhoneSetAlsBlocked=IPC_GSM_CTM_EXT, //10000
  EGsmPhoneSetAlsBlockedCancel, //10001
  EGsmPhoneGetAlsBlocked, //10002
  EGsmPhoneGetAlsBlockedCancel, //10003
  EGsmPhone, //10004
  EGsmPhone, //10005
  EGsmPhone, //10006
  EGsmPhone, //10007
  EGsmPhone, //10008
  EGsmPhone, //10009
  EGsmPhone, //10010
  EGsmPhone, //10011
  EGsmPhone, //10012
  EGsmPhone, //10013
  EGsmPhone, //10014
  EGsmPhone, //10015
  EGsmPhone, //10016
  EGsmPhone, //10017
  EGsmPhone, //10018
  EGsmPhone, //10019
  EGsmPhone, //10020
  EGsmPhone, //10021
  EGsmPhone, //10022
  EGsmPhone, //10023
  EGsmPhone, //10024
  EGsmPhone, //10025
  EGsmPhone, //10026
  EGsmPhone, //10027
  EGsmPhone, //10028
  EGsmPhone, //10029
  EGsmPhone, //10030
  EGsmPhone, //10031
  EGsmPhoneSetHandsfreeMode, //10032
  EGsmPhone, //10033
  EGsmPhoneSetHandsfreeModeCancel, //10034
  EGsmPhoneGetHandsfreeMode, //10035
  EGsmPhone, //10036
  EGsmPhoneGetHandsfreeModeCancel, //10037
  EGsmPhone, //10038
  EGsmPhone, //10039
  EGsmPhone, //10040
  EGsmPhone, //10041
  EGsmPhone, //10042
  EGsmPhone, //10043
  EGsmPhone, //10044
  EGsmPhone, //10045
  EGsmPhone, //10046
  EGsmPhone, //10047
  EGsmPhone, //10048
  EGsmPhone, //10049
  EGsmPhone, //10050
  EGsmPhone, //10051
  EGsmPhone, //10052
  EGsmPhone, //10053
  EGsmPhone, //10054
  EGsmPhone, //10055
  EGsmPhone, //10056
  EGsmPhoneGetOutputVolume, //10057
  EGsmPhoneGetOutputVolumeCancel, //10058
  EGsmPhoneSetOutputVolume, //10059
  EGsmPhoneSetOutputVolumeCancel, //10060
  EGsmPhoneNotifyOutputVolumeChange, //10061
  EGsmPhoneNotifyOutputVolumeChangeCancel, //10062
  EGsmPhoneGetVolMuteState, //10063
  EGsmPhoneGetVolMuteStateCancel, //10064
  EGsmPhoneSetVolMuteState, //10065
  EGsmPhoneSetVolMuteStateCancel, //10066
  EGsmPhoneSetAccessoryMode, //10067
  EGsmPhoneSetAccessoryModeCancel, //10068
  EGsmPhoneGetAccessoryMode, //10069
  EGsmPhoneGetAccessoryModeCancel, //10070
  EGsmPhoneNotifyAccessoryModeChange, //10071
  EGsmPhoneNotifyAccessoryModeChangeCancel, //10072
  EGsmPhoneGetLoopSetModeState, //10073
  EGsmPhoneGetLoopSetModeStateCancel, //10074
  EGsmPhoneSetLoopSetModeState, //10075
  EGsmPhoneSetLoopSetModeStateCancel, //10076
  EGsmPhoneCallEventNotification, //10077
  EGsmPhoneCallEventNotificationCancel, //10078
  EGsmPhoneSsNetworkNotification, //10079
  EGsmPhoneSsNetworkNotificationCancel, //10080
  EGsmPhoneGetBasicServiceCode, //10081
  EGsmPhoneGetBasicServiceCodeCancel, //10082
  EGsmPhone, //10083
  EGsmPhone, //10084
  EGsmPhone, //10085
  EGsmPhone, //10086
  EGsmPhone, //10087
  EGsmPhone, //10088
  EGsmPhone, //10089
  EGsmPhone, //10090
  EGsmPhone, //10091
  EGsmPhone, //10092
  EGsmPhone, //10093
  EGsmPhone, //10094
  EGsmPhone, //10095
  EGsmPhone, //10096
  EGsmPhone, //10097
  EGsmPhone, //10098
  EGsmPhone, //10099
  EGsmPhone, //10100
  EGsmPhone, //10101
  EGsmPhone, //10102
  EGsmPhone, //10103
  EGsmPhone, //10104
  EGsmPhone, //10105
  EGsmPhone, //10106
  EGsmPhone, //10107
  EGsmPhone, //10108
  EGsmPhone, //10109
  EGsmPhone, //10110
  EGsmPhone, //10111
  EGsmPhone, //10112
  EGsmPhone, //10113
  EGsmPhone, //10114
  EGsmPhone, //10115
  EGsmPhone, //10116
  EGsmPhone, //10117
  EGsmPhone, //10118
  EGsmPhone, //10119
  EGsmPhone, //10120
  EGsmPhone, //10121
  EGsmPhone, //10122
  EGsmPhone, //10123
  EGsmPhone, //10124
  EGsmPhone, //10125
  EGsmPhone, //10126
  EGsmPhone, //10127
  EGsmPhone, //10128
  EGsmPhone, //10129
  EGsmPhone, //10130
  EGsmPhone, //10131
  EGsmPhone, //10132
  EGsmPhone, //10133
  EGsmPhone, //10134
  EGsmPhone, //10135
  EGsmPhone, //10136
  EGsmPhone, //10137
  EGsmPhone, //10138
  EGsmPhone, //10139
  EGsmPhone, //10140
  EGsmPhone, //10141
  EGsmPhone, //10142
  EGsmPhone, //10143
  EGsmPhone, //10144
  EGsmPhone, //10145
  EGsmPhone, //10146
  EGsmPhone, //10147
  EGsmPhone, //10148
  EGsmPhone, //10149
  EGsmPhone, //10150
  EGsmPhone, //10151
  EGsmPhone, //10152
  EGsmPhone, //10153
  EGsmPhone, //10154
  EGsmPhone, //10155
  EGsmPhone, //10156
  EGsmPhone, //10157
  EGsmPhone, //10158
  EGsmPhone, //10159
  EGsmPhone, //10160
  EGsmPhone, //10161
  EGsmPhone, //10162
  EGsmPhone, //10163
  EGsmPhone, //10164
  EGsmPhone, //10165
  EGsmPhone, //10166
  EGsmPhone, //10167
  EGsmPhone, //10168
  EGsmPhone, //10169
  EGsmPhone, //10170
  EGsmPhone, //10171
  EGsmPhone, //10172
  EGsmPhone, //10173
  EGsmPhone, //10174
  EGsmPhone, //10175
  EGsmPhone, //10176
  EGsmPhone, //10177
  EGsmPhone, //10178
  EGsmPhone, //10179
  EGsmPhone, //10180
  EGsmPhone, //10181
  EGsmPhone, //10182
  EGsmPhone, //10183
  EGsmPhone, //10184
  EGsmPhone, //10185
  EGsmPhone, //10186
  EGsmPhone, //10187
  EGsmPhone, //10188
  EGsmPhone, //10189
  EGsmPhone, //10190
  EGsmPhone, //10191
  EGsmPhone, //10192
  EGsmPhone, //10193
  EGsmPhone, //10194
  EGsmPhone, //10195
  EGsmPhone, //10196
  EGsmPhone, //10197
  EGsmPhone, //10198
  EGsmPhone, //10199
  EGsmPhone, //10200
  EGsmPhone, //10201
  EGsmPhone, //10202
  EGsmPhone, //10203
  EGsmPhone, //10204
  EGsmPhone, //10205
  EGsmPhone, //10206
  EGsmPhone, //10207
  EGsmPhone, //10208
  EGsmPhone, //10209
  EGsmPhone, //10210
  EGsmPhone, //10211
  EGsmPhone, //10212
  EGsmPhone, //10213
  EGsmPhone, //10214
  EGsmPhone, //10215
  EGsmPhone, //10216
  EGsmPhone, //10217
  EGsmPhone, //10218
  EGsmPhone, //10219
  EGsmPhone, //10220
  EGsmPhone, //10221
  EGsmPhone, //10222
  EGsmPhone, //10223
  EGsmPhone, //10224
  EGsmPhone, //10225
  EGsmPhone, //10226
  EGsmPhone, //10227
  EGsmPhone, //10228
  EGsmPhone, //10229
  EGsmPhone, //10230
  EGsmPhone, //10231
  EGsmPhone, //10232
  EGsmPhone, //10233
  EGsmPhone, //10234
  EGsmPhone, //10235
  EGsmPhone, //10236
  EGsmPhone, //10237
  EGsmPhone, //10238
  EGsmPhone, //10239
  EGsmPhone, //10240
  EGsmPhone, //10241
  EGsmPhone, //10242
  EGsmPhone, //10243
  EGsmPhone, //10244
  EGsmPhone, //10245
  EGsmPhone, //10246
  EGsmPhone, //10247
  EGsmPhone, //10248
  EGsmPhone, //10249
  EGsmPhone, //10250
  EGsmPhone, //10251
  EGsmPhone, //10252
  EGsmPhone, //10253
  EGsmPhone, //10254
  EGsmPhone, //10255
  EGsmPhone, //10256
  EGsmPhone, //10257
  EGsmPhone, //10258
  EGsmPhone, //10259
  EGsmPhone, //10260
  EGsmPhone, //10261
  EGsmPhone, //10262
  EGsmPhone, //10263
  EGsmPhone, //10264
  EGsmPhone, //10265
  EGsmPhone, //10266
  EGsmPhone, //10267
  EGsmPhone, //10268
  EGsmPhone, //10269
  EGsmPhone, //10270
  EGsmPhone, //10271
  EGsmPhone, //10272
  EGsmPhone, //10273
  EGsmPhone, //10274
  EGsmPhone, //10275
  EGsmPhone, //10276
  EGsmPhone, //10277
  EGsmPhone, //10278
  EGsmPhone, //10279
  EGsmPhone, //10280
  EGsmPhone, //10281
  EGsmPhone, //10282
  EGsmPhone, //10283
  EGsmPhone, //10284
  EGsmPhone, //10285
  EGsmPhone, //10286
  EGsmPhone, //10287
  EGsmPhone, //10288
  EGsmPhone, //10289
  EGsmPhone, //10290
  EGsmPhone, //10291
  EGsmPhone, //10292
  EGsmPhone, //10293
  EGsmPhone, //10294
  EGsmPhone, //10295
  EGsmPhone, //10296
  EGsmPhone, //10297
  EGsmPhone, //10298
  EGsmPhone //10299
};

#endif