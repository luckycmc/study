# include <stdio.h>

const int MAX = 3;

int main()
{
	 int var[] = {10,100,200};
	 int i,*ptr;
	 
	 //ptr 指向元素的首地址
	 /*ptr = var;
	 
	 for(i =0; i< MAX ;i++)
	 {
	 	  printf("存储地址：var[%d] = %p\n", i, ptr );
	 	  
          printf("存储值：var[%d] = %d\n", i, *ptr );  //通过指针显示对应的数据 
 
           /* 指向下一个位置  //指针下移更具当前的数据类型 而选择移动多少个字节 
         ptr++;
	 } */
	 
	 //指针递减运算
	 /*ptr = &var[MAX];   //获取最后一个元素的地址 
	 
	 for(i = MAX ;i>0; i--)
	 {
	 	 printf("存储地址：var[%d] = %p\n", i-1, ptr );
	 	 
          printf("存储值：var[%d] = %d\n", i-1, *ptr );
          
          //指针从最大的节点向下移动
		  ptr--; 
	 } */
	 ptr = &var;  //获取指针中的第一个地址
	 i = 0;
	 while(ptr <= &var[MAX-1])
	 {
	 	  printf("存储地址：var[%d] = %p\n", i, ptr );
          printf("存储值：var[%d] = %d\n", i, *ptr );
 
          /* 指向上一个位置 */
          ptr++;  //指针向后移动 
          i++;
	 } 
	 return  0;
}
@ B/92              ~             @ B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                胒fffff.�     H冹(H�u4  1仪    H�v4  �    H�y4  �    H�<4  �    H��2  f�8MZtxH�4  �  � 吚tW�   鑃  H橇�����/  H�H3  H�H�.3  H�H��3  �H��2  H� ��  H�:2  �8te1繦兡(霉   椟  毵f怘cH<H葋8PE  卽���稨f侚tEf侚卂���兏�   哛���媹�   1覅�暵锧����     H�
	  梏  1繦兡(脙xt����D媭�   1褽吚暵����ffffff.�     H冹8H��2  L�  H�  H�
╚  � �x^  H�q^  H塂$ H��2  D��-  �g^  H兡8胒怉UATUWVSH侅�   H�-�2  1拦
   H峊$ D婱 H壸驢獷吷叓  eH�%0   H��1  H媝1�L�%zp  �H9��#  硅  A�訦夬餒�3H吚u釮�5�1  1��凐�  �吚刞  �篯     �凐�  ���#  H��0  H� H吚tE1篮   1��需�  H�
,
  �阰  H��0  H��'  H�
`���鐺  杵  H��0  H�`g  H��0  H� H� H吚tM1呻f悇襱<冡t+�   H兝�€� ~鍭壢A凁€�"AD入鋐D  勔tH兝�€� ~餒�鵩  D婨 E吚t鯠$\�
   咇   ��  D�%隲  E峫$Mc鞩铃L夐璇  E呬H壟H�=耚  嶼  1踗.�     H�哞�  H峱H夞鑬  H塂� H�逪壛I夝H兠鑣  A9�蜪冺J荄-     H�-j\  鑕  H��/  H�O\  �
Y\  H� H�L�<\  H�=\  鐷  �
\  �\  吷勈   �\  呉u�
  �鸞  H伳�   [^_]A\A]�稤$`����D  H�5y/  �   �凐咃����   柘  �凐咘���H��/  H�
v/  韫  ���   呧��� 1繦�橛���fD  H壯�媘  镚���f怘�i/  H�
R/  �   鑟  閷���E1黹帼��壛鑓  恌怘冹(H��/  �    鑊
  枧���悙H兡(胒ffff.�     H冹(H�e/  �     �*
  钑���悙H兡(脨悙悙悙悙悙悙怳H夊H冹0桡  荅�   婨鼔翲�
�  �   �    H兡0]肬H夊H冹@H塎塙荅�    ��*  塃羟E�    闃   婨麳c蠬壭H繦蠬拎H壜H婨H畜@�M趔X萬~葔E魦E麳c蠬壭H繦蠬拎H壜H婨H畜H�q*  .羦僂��
   �>  婨麳c蠬壭H繦蠬拎H壜H婨H蠬壛�!  僂�婨�;E孿����E趔
 *  �^馏Z润ZE魦U鴉H~菻壛H塎栩M鐷壛fH~繧壚L塃栩E鐰壯f(袸壢f(菻壜H�
�)  璨  怘兡@]脨悙悙悙悙悙H壢胒ff.�     H壢脨悙悙悙悙悙�1烂悙悙悙悙悙悙怘冹XH�eY  H吚t,��$€   塋$ H峀$ H塗$(�T$0�\$8�D$@�袗H兡X胒fffff.�     H�
Y  �  @ VSH冹x)t$@)|$PD)D$`�9囇   �H�,*  Hc怘��郒��(  �     駾A H媞�y�q枨  駾D$0H岺`H��)  �|$(I夞I壺�t$ 瑕  �(t$@1�(|$PD(D$`H兡x[^�@ H��(  霔€    H��(  雵€    H�)  閠���@ H��(  閐���@ H��(  門���@ H�)  镈���悙悙VSH冹8H岲$XH壦H塗$XL塂$`L塋$hH塂$(棂  H�
B)  L岺`A�   �   桀  H媡$(柙  H岺`H壼I夝栎  栲  恌fffff.�     AUATUWVSH冹XHc-  H壦H壷L壡呿庼   H�峎  E1蒆兝L�L9觬H婸D媄M贚9�偛   A兞H兝A9閡譎壻栎
  H吚I壞勩   H峫m H铃I夗L-6W  I塃A荅     柰  A婽$A�0   H蠬峊$ I塃H�W  H婰(�Hi  H吚tt婦$D峆鼉恹t,冭@冟縯$I夐L
賄  A窣   H婽$8H婰$ �i  吚t(�碫  I夬H夠H壻H兡X[^_]A\A]閂  1黹:����Uh  H�
J(  壜鑋���H�|V  A婽$H�
�'  L婦(�>���H�
�'  H壼�/���恌ffff.�     UAVAUATWVSH夊H冹`�)V  呟tH夓[^_A\A]A^]�€    �V     栳  H楬�@H��   H冟痂  H�5�(  H��(  H)那覷      H岲$ H�薝  H夝H)豀凐~扝凐~&D�E呟匛  D婼E呉�8  D婯E吷uH兠D�E吚�  婯吷�  婼凓�  H兠H9��6���L�%D(  L峬蠭�    ����婯�禨L郘醿�L� 凙  �  凓 剓  凓@區  I)繪夑L夛L壚HA�   H塃需E���H兠H9髍闠  1�1鰠�幍���L�5'g  L峬蘈�%g  �fD  兤H兠;5稵  崍���H壺H玊  �呉t軭婬A�0   H夲A�諬吚�  H�匱  M夐H婾鐷婱蠨�A�噪ゝ�     H9��2���L�%@'  H峿袐K�A�   H夲H兠L�塃需}���H9髍荛7���凓ui�L夛I壱I伿 ���勔IH襀)翴�A�   L夑H塃需?���轷����L夛I壱I伿  ��f呉IH襀)翴�A�   L夑H塃需
���槔���H�
�%  H荅�    鑱����L夛I壱M	騾襂H襀)翴�A�   L夑H塃需躯��閩���HsS  H�
�$  H婥L婥婸�4���H�
=%  �(���悙悙悙悙H冹(�=�  纖c=�  纒{=  ��  囒   =  €�   =  �呌   1夜   �!  H凐�?  H吚�  �   ��1繦兡(��     =�  �勁   wC=�  �劏   =�  纔1夜   柰  H凐H壜ttH呉�   t倒   ��1繦兡(�@ =�  纓i=�  纔@1夜   鑾  H凐剸   H吚剝   �   ��1篱h���D  =  纓�=�  纓"�   H兡(煤   �   鐰  �4  @ 1繦兡(胒�     1夜   �  H凐H壜匥����   �   �   1篱酤��惛   H兡(煤   �   柽
  1篱誉���   �   枭
  1篱浸��f.�     ATUWVSH冹 �1	  H壟�癚  吚u%H呿t H�
�#  �朡     鐸  H吚t�   H兡 [^_]A\�@ H�㏑  �0   H�漄  L�%纨��1鯤夁驢獺壸I)毂 驢獺壸�.�	D塯H兤婸H兠塖�PH夬H)鐷兦塁鼔S鳫凗 t5H夞桫  H吚u臜咑剉���夠�     I夎H�
&R  �碽  閃����    脘@ SH冹 H��A壚A佮��� A侙CCG ��   =�  繦壦噥   =�  �儘   =  纓2w==  €t)=  纔A1夜   鑛  H凐�)  H吚t&�   �懈����H兡 [胒�=  �劵   =�  纓酘�7P  H吚t贖壻H兡 [H��€    =�  纓;ww=�  纓�=�  纔�1夜   桴  H凐u+�   �   栲  栌   閬���1夜   枋  H凐tnH吚t壒   �懈����閊���fD  鯞咓���镈����=�  ��8���=�  �匧���1夜   鑦  H凐tLH吚�1����   �懈���������   �   鐶  轫���f.�     �   �   �)  橄����   �   �  榛���悙悙悙悙坫脨悙悙悙悙悙悙H冹(H�5  H� H吚t�蠬�$  H峆H婡H�  H吚u鉎兡(胒f.�     VSH冹(H�53!  H�凐�壝t)呟tD  壺�苾�u鯤�
����H兡([^閯  @ 1垭壝峉H�<� H壭u耠耭f.�     �Q  吚t竺@ �Q     雱悙悙ATUWVSH冹@H��  H�2⑦-�+  H荄$     H9胻H饔H��  H兡@[^_]A\胒怘峀$ �  L媎$ �v_  壟�v_  壡�巁  H峀$0壠�檁  L3d$0夲H�������  L1錒夛H1讐騂1譎夻H!艸9辴#H夝H餍H�56  H�?  H兡@[^_]A\�@ H柑] 襢���H�3⑦-�+  胪f.�     UVSH夊H冹pH壩H�
LP  �"_  H�7Q  E1繦峌豀壻�_  H吚I壛劊   H岴郒荄$8    I壺H婾�1蒆塂$0H岴鐷塂$(H�鳲  H塂$ �賌  H�轕  1蒆�5]P  �覶  	 狼蚑     H�襎  H�[
  H塃餒�`
  H塃��歗  H�
�  �璣  �^  �	 繦壛�塣  鑠  H婨H�mP  H岴H�P  閤���悙悙怘冹(呉t凓t�   H兡(胒�     铔  �   H兡(脨VSH冹(H��  �8t�    凓t凓t@�   H兡([^胒怘�賙  H�5襨  H9髏逪�H吚t�蠬兠H9髐砀   H兡([^描)  牍€    1烂悙悙悙悙悙悙怱H冹0H壦H�
QU  杓���H凐�H塂$ tv�   鑷  H�
0U  铔���H�
,U  H塂$ 鑺���H峊$ L岲$(H壻H塂$(鑋  H婰$ H壝鑦���H婰$(H�闠  鑕����   H�酺  �4  H壺H兡0[肏�  H壻�H兡0[胒�     H冹(鐶���H吚斃独髫H兡(脨悙悙悙H�5]  H�鄲悙悙怘cA<H�1纴9PE  t竺1纅亂斃脨f�9MZt	1烂fD  胛fffff.�     HcA<H�稟H岲稩吷t)冮H�塋峀�(D婡I9蠰壛wHH9蕆H兝(L9萿�1荔胒f.�     WVSH冹 H壩韫  H凐wH��  1�f�;MZtH夬H兡 [^_�1�H夬H兡 [^_� H壻�(���吚t貶cS<H�稡H峔稲呉t羶�H�扝峾�(�	H兠(H9鹴礎�   H夠H壻柢  吚u釮夁H夬H兡 [^_胒怴SH冹(H�S  f�;MZt1繦兡([^� H壩H壻瑗���吚t錒)轍壻H夠H兡([^橥���ffff.�     SH冹 H�  1纅�;MZtH兡 [�D  H壻鑈���吚t镠cC<稤H兡 [�D  WVSH冹 H�5�  1�H壦f�>MZtH夬H兡 [^_肏夞����吚t镠cF<H�種稦吷H峊t褍�H�塇岲�(鯞' t	H呟t H冸H兟(H9聈�1�H夬H兡 [^_胒D  H壸H夬H兡 [^_胒怴SH冹(H�53  1踗�>MZt
H壺H兡([^肏夞鑸���吚HE轍壺H兡([^��     VSH冹(H��  1纅�;MZt
H兡([^� H壩H壻鐴���吚t鏗)轍壻H夠鑣���H吚t婡$餍凌H兡([^�1离羏f.�     UWVSH冹(H�=�  1韷蝔�?MZtH夎H兡([^_]胒.�     H夰柝���吚t轍cG<嫈�   呉t蠅親夰H壼桫���H吚t紿鸋壼u氤f�     冾H兟婮吷u婤吚t咑鐙jH鼿夎H兡([^_]�D  1鞨夎H兡([^_]脨怳WVSH冹(H�
aP  �梄  H�4P  H呟t3H�-Y  H�=漍  悑�誋壠�讌纔H咑t	H婥H夞�蠬媅H呟u蹾�橷  H�
P  H兡([^_]H�� WVSH冹 �跲  吚uH兡 [^_�€    H壷壪�   �   杓  H吚H壝t<�8H塸H�
稯  �鞼  H�奜  H�
  H�|O  H塁�X  1繦兡 [^_酶����霋怱H冹 �]O  吚u1繦兡 [胒�     壦H�
WO  �峎  H�*O  H呉t�9豼
隠D� A9豻%H壜H婤H吚u霩�
"O  �燱  1繦兡 [�@ H壛H婡H塀桫  H�
鵑  �wW  胝H婤H壯H�肗  胲怘冹(凓tIr凓u
�睳  吚uf�   H兡(脣濶  吚ub�擭  凐u酘�
燦  �~N      �腣  肴�pN  吚t�   �]N     H兡(肏�
iN  �遃  胼愯臊��霌f�     枸���霔悙悙悙悙怮PH=   H峀$rH侀   H�	 H-   H=   w鏗)罤�	 XY脨悙悙悙悙悙悙��%頥  悙�%W  悙�%頥  悙�     H�N  ��     H壢H�N  脨悙悙�%^W  悙�%~W  悙�%VW  悙�%鎂  悙�%諺  悙�%鎂  悙�%W  悙�%>W  悙�%.W  悙�%濾  悙�%~V  悙�%頥  悙�%鯲  悙�%.W  悙�%綱  悙�%W  悙�%朧  悙�%.V  悙�%朧  悙�%哣  悙�%6V  悙�%鎂  悙�%哣  悙�%朧  悙�%禪  悙�%  悙�%NU  悙�%芔  悙�%禪  悙�%.U  悙�%^U  悙�%.U  悙�%U  悙�%U  悙�%U  悙�%.U  悙�%6U  悙�%6U  悙�%6U  悙�%VU  悙�%綯  悙�%6U  悙�%  悙�%.U  悙�%轙  悙�%員  悙�%芓  悙�     ��������        ��������                                                                                                        
                                                                @@           C     C@@           A    €C@@           A    €C@@           F     C&@@           B    €C        �-@             ��������        �                              ����            �+@     �+@     2⑦-�+          蚞 襢���                                                                                                                                                                                                        Li ping Zhang ping He fang Cheng ling Wang ming sum=%.2f
average=%.2f
num_140=%d
         C  燖Argument domain error (DOMAIN) Argument singularity (SIGN)      Overflow range error (OVERFLOW) Partial loss of significance (PLOSS)    Total loss of significance (TLOSS)      The result is too small to be represented (UNDERFLOW) Unknown error     _matherr(): %s in %s(%g, %g)  (retval=%g)
  溨��菡��L���\���l���|���屩��Mingw-w64 runtime failure:
     Address %p has no image-section   VirtualQuery failed for %d bytes at address %p          VirtualProtect failed with code 0x%x    Unknown pseudo relocation protocol version %d.
         Unknown pseudo relocation bit size %d.
               .pdata          €x@     €s@     P$@             �0@              1@             €-@              H@              H@             蠦@               @             虃@             魝@             傽             $傽             €p@             (z@              z@             0z@             8z@              怈             怈             怈             0怈             Py@             py@             �@             0@             `y@             0p@             €y@             Hy@             Dy@             @y@             GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (tdm64-1) 4.9.2            GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (tdm64-1) 4.9.2            GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                GCC: (GNU) 4.9.2                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      `    Q  `  `  �  `  �  �  `  �  �  (`     "  H`  0  `  h`  `  �  t`  �  �  €`  �  �  刞  �  �  坄  �    宍     ,  擿  0  ,  榒  0  �  癭  �    糮    	  衊    �  鑐  �  �  餪  �  h!   a  p!  s!  a  €!  �!  a  �!  "  a   "  <"   a  @"  #  $a   #  $  4a   $  O$  Da  P$  �$  La  �$  �$  Xa  �$  w%  \a  €%  �%  da  �%  �%  la  �%  �%  pa  �%  �%  ta  �%  5&  xa  @&  �&  |a  �&  #'  坅  0'  k'  攁  p'  �'  渁   (  8(  ╝  @(  �(  碼  �(  N)  繿  P)  �)  衋  �)  ?*  郺  @*  �*  靉  �*  w+  鬭  �+  �+  黙  �+  �+   b                                                                                                                                                                                                                                                                                                                                                                                                                                                                      B   b    0`pP��	 B  �+     �  �  �  �  	 B  �+         �    RP  rP            �     	 � x h �0`   b0`   �0`pP��  	�

0	`p���P   B  
 
20`pP� 20    B   B0`     
 
r0`pP�

�0`P   B   B0`      R0 B               20`p B0`   20 20`p B0`   B0`   B0`pP   B0`pP   20`p 20 B                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    <€          T�  魜  鼆          車  磦                      瑑      膬      軆      饍      �      �      ,�      >�      X�      h�      剟      渼      秳      虅      鄤      鷦      �      ,�      4�      H�      V�      r�      剠              攨      瑓      簠      蕝      謪      鋮      魠      �      �      $�      2�      <�      F�      R�      Z�      d�      n�      v�      €�      垎      拞      殕            畣      竼      聠      虇      謫      鄦      陠              瑑      膬      軆      饍      �      �      ,�      >�      X�      h�      剟      渼      秳      虅      鄤      鷦      �      ,�      4�      H�      V�      r�      剠              攨      瑓      簠      蕝      謪      鋮      魠      �      �      $�      2�      <�      F�      R�      Z�      d�      n�      v�      €�      垎      拞      殕            畣      竼      聠      虇      謫      鄦      陠              � DeleteCriticalSection � EnterCriticalSection  �GetCurrentProcess �GetCurrentProcessId �GetCurrentThreadId  GetLastError  sGetStartupInfoA �GetSystemTimeAsFileTime �GetTickCount  �InitializeCriticalSection KLeaveCriticalSection  �QueryPerformanceCounter RtlAddFunctionTable RtlCaptureContext 	RtlLookupFunctionEntry  RtlVirtualUnwind  �SetUnhandledExceptionFilter �Sleep �TerminateProcess  �TlsGetValue �UnhandledExceptionFilter  �VirtualProtect  �VirtualQuery  7 __C_specific_handler  N __dllonexit Q __getmainargs R __initenv S __iob_func  Z __lconv_init  ` __set_app_type  b __setusermatherr  t _acmdln { _amsg_exit  � _cexit  � _fmode  I_initterm �_lock b_onexit 2_unlock abort calloc  exit  0fprintf 7free  Bfwrite  qmalloc  ymemcpy  €printf  �putchar �signal  �strlen  �strncmp �vfprintf     €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €   €  KERNEL32.dll    €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  €  msvcrt.dll                                  `@                     @     �%@                     P$@      $@                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      燖     `燖     Ly@     @怈                                                                                                                                                                                                                                                                                                                                                                                                                                                                     ,              @     "                      ,    �'       �@                            ,    �,       �@                            ,    ".       �@     \                      ,    5       0@     �                      ,    @G       @     X                      ,    嶾       p!@                            ,    L]       €!@     �                       ,    鬮       @"@     �                      ,    mq        $@     �                           Hx                           鷜                           )                           �                           巰                       ,    =�       �$@     �                       ,    �       �%@     
                       ,    ��       �%@     �                          q�                       ,    t�       P)@     '                          �                       ,    暴       €+@     2                           [�                           O�                       ,    �       �+@                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            �'       GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/crtexe.c  @     "          char size_t #�   long long unsigned int long long int uintptr_t K�   wchar_t b�   short unsigned int int long int �     unsigned int sizetype long unsigned int unsigned char {  _EXCEPTION_RECORD ��	%  ExceptionCode �	A   ExceptionFlags �	A  �  �	u  ExceptionAddress �	�  NumberParameters �	A  ExceptionInformation �	P
    -  	_CONTEXT ��  P1Home ��   P2Home ��  P3Home ��  P4Home ��  P5Home ��   P6Home ��  (ContextFlags �A  0MxCsr �A  4SegCs �5  8SegDs �5  :SegEs �5  <SegFs �5  >SegGs �5  @SegSs �5  BEFlags �A  DDr0 ��  HDr1 ��  PDr2 ��  XDr3 ��  `Dr6 ��  hDr7 ��  pRax ��  xRcx ��  €Rdx ��  �Rbx ��  �Rsp ��  �Rbp ��  �Rsi ��  �Rdi ��  �R8 ��  �R9 ��  �R10 ��  �R11 ��  �R12 ��  �R13 ��  �R14 ��  �R15 ��  �Rip ��  �
�	   VectorRegister �/
   VectorControl ��  �DebugControl ��  �LastBranchToRip ��  �LastBranchFromRip ��  �LastExceptionToRip ��  �LastExceptionFromRip ��  � WINBOOL   BYTE 媎  WORD 岨   DWORD 峅  float )  LPBYTE 慦  q  signed char short int ULONG_PTR 1�   DWORD64 鲁   PVOID �%  
CHAR �   
LONG   �  
LPSTR I�  
HANDLE �%  
LONGLONG ��   
ULONGLONG ��   
EXCEPTION_ROUTINE �5    S  u  �  '  �   
PEXCEPTION_ROUTINE �n    _M128A D�  Low E	   High F�   
M128A Gt  �  �  C   �  �  C   )  �  C  _ �    double long double   _invalid_parameter_handler �+  1  P  P  P  P  3  �    V  �   	_XMM_SAVE_AREA32  l�  ControlWord m5   StatusWord n5  TagWord o)  Reserved1 p)  ErrorOpcode q5  ErrorOffset rA  ErrorSelector s5  Reserved2 t5  DataOffset uA  DataSelector v5  Reserved3 w5  MxCsr xA  MxCsr_Mask yA  FloatRegisters z�   XmmRegisters {�  �Reserved4 |�  � 
XMM_SAVE_AREA32 }[  ���	  Header ��	   Legacy ��   Xmm0 ��  �Xmm1 ��  �Xmm2 ��  �Xmm3 ��  �Xmm4 ��  �Xmm5 ��  �Xmm6 ��   Xmm7 ��  Xmm8 ��   Xmm9 ��  0Xmm10 ��  @Xmm11 ��  PXmm12 ��  `Xmm13 ��  pXmm14 ��  €Xmm15 ��  � �  �	  C    �/
  FltSave ��  FloatSave ��  �   �  ?
  C   
PCONTEXT �'  �  `
  C   
EXCEPTION_RECORD �	{  
PEXCEPTION_RECORD �	�
  `
  _EXCEPTION_POINTERS �	�
  �  �	y
   ContextRecord �	?
   �
  �  Next �:  prev �:   _EXCEPTION_REGISTRATION_RECORD �:  �
   @     �j  Handler �S  handler �S   ��  FiberData ��  Version �A   _NT_TIB 8�.  ExceptionList �:   StackBase ��  StackLimit ��  SubSystemTib ��  j   ArbitraryUserPointer ��  (Self �.  0 �  
NT_TIB ��  
PNT_TIB �S  4  _IMAGE_DOS_HEADER @��
  e_magic �5   e_cblp �5  e_cp �5  e_crlc �5  e_cparhdr �5  e_minalloc �5  
e_maxalloc �5  e_ss �5  e_sp �5  e_csum �5  e_ip �5  e_cs �5  e_lfarlc �5  e_ovno �5  e_res ��
  e_oemid �5  $e_oeminfo �5  &e_res2 ��
  (e_lfanew ��  < 5  �
  C   5  �
  C  	 
IMAGE_DOS_HEADER �Y  
PIMAGE_DOS_HEADER �  Y  _IMAGE_FILE_HEADER  �  Machine !5   NumberOfSections "5  TimeDateStamp #A  PointerToSymbolTable $A  NumberOfSymbols %A  SizeOfOptionalHeader &5  Characteristics '5   
IMAGE_FILE_HEADER (  _IMAGE_DATA_DIRECTORY \4  VirtualAddress ]A   Size ^A   
IMAGE_DATA_DIRECTORY _�  _IMAGE_OPTIONAL_HEADER �c  Magic e5   �   f)  �   g)  {   hA  �   iA    jA  (  kA     lA  BaseOfData mA  q   nA  *   oA   |  pA  $`  q5  (�  r5  *
  s5  ,�  t5  .<  u5  0;   v5  2�  wA  4   xA  8R  yA  <
   zA  @    {5  D�  |5  F�   }A  H_   ~A  L�   A  P�   €A  T�  �A  X�  �A  \Q   �  ` 4    C   
PIMAGE_OPTIONAL_HEADER32 �?  Q  _IMAGE_OPTIONAL_HEADER64 ���  Magic �5   �   �)  �   �)  {   �A  �   �A    �A  (  �A     �A  q   �	  *   �A   |  �A  $`  �5  (�  �5  *
  �5  ,�  �5  .<  �5  0;   �5  2�  �A  4   �A  8R  �A  <
   �A  @    �5  D�  �5  F�   �	  H_   �	  P�   �	  X�   �	  `�  �A  h�  �A  lQ   �  p 
IMAGE_OPTIONAL_HEADER64 �E  
PIMAGE_OPTIONAL_HEADER64 �1  E  	_IMAGE_NT_HEADERS64 ��  Signature �A   FileHeader ��  OptionalHeader ��   
PIMAGE_NT_HEADERS64 ��  7  
PIMAGE_NT_HEADERS ��  
PIMAGE_TLS_CALLBACK �  �    �  A  �     �    �
   HINSTANCE__ 蠥  unused �    HINSTANCE 蠷    PTOP_LEVEL_EXCEPTION_FILTER   LPTOP_LEVEL_EXCEPTION_FILTER X  _STARTUPINFOA h	�  cb 	 A   lpReserved 	!�  lpDesktop 	"�  lpTitle 	#�  dwX 	$A   dwY 	%A  $dwXSize 	&A  (dwYSize 	'A  ,dwXCountChars 	(A  0dwYCountChars 	)A  4dwFillAttribute 	*A  8dwFlags 	+A  <wShowWindow 	,5  @cbReserved2 	-5  BlpReserved2 	.]  HhStdInput 	/�  PhStdOutput 	0�  XhStdError 	1�  ` STARTUPINFOA 	2�  STARTUPINFO 	I�  '  tagCOINITBASE 
昑  COINITBASE_MULTITHREADED    VARENUM  �  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � _PVFV   _PIFV �  !�  newmode �    _startupinfo �  "�   漮  __uninitialized  __initializing __initialized  #�   �/  _exception (
ㄑ  type 
�   name 
  arg1 
  arg2 
  retval 
    �  �   
_TCHAR ��   $__readgsqword ��   "  %Offset �O  &ret ��    '_TEB (NtCurrentTeb � >  "  $_InterlockedCompareExchangePointer �%  �  %Destination ��  %ExChange �%  %Comperand �%   �  )%  $_InterlockedExchangePointer �%  �  %Target ��  %Value �%   *duplicate_ppstrings �^  %ac �  %av �^  &avl �  &i �  &n �  +&l ��       ,__mingw_invalidParameterHandler q @            滛  -expression qP  R-function rP  Q-file sP  X-line t3  Y-pReserved u�   �  .check_managed_app ]  a  &pDOSHeader _�
  &pPEHeader `�  &pNTHeader32 a  &pNTHeader64 b   /pre_c_init ~  @     A      �,  0�  @         €�  1    2  3&      38  6   3L  p     4u@     %  �  5R2 4�@     �%  �  5R	� 6�@     �%  4�@     %    5R1 7,@     �%  8R  ,pre_cpp_init 沗@     N       湒  7�@     &  5R	(p@     5Q	 p@     5X	p@     5w 	 p@       /__tmainCRTStartup �  �@           溦   9lpszCommandLine 嗾   �   :StartupInfo �	  懶~9inDoubleQuote �  �   ;@   �   9lock_free �%  U  9fiberid �%  y  9nested �  �  <(  �@            衄  =�  �@            � >  3  ?�@            3  m     0D  �@     €   鲔  >�  �  >�  �  @t   A�  =@     �   C�  >&  �  >  F  1�   31  |  3=  �  3G  �  Bp@     -       }  3R  Y  6y@     D&  4�@     \&  h  5Rt  7�@     u&  5Xt   7Q@     \&  5R}    C�  €@            �  >�  |  @�   D@     �  5R
� Dx@     �  5R05Q25X0 6}@     �&  D�@        8R 6�@     �&  4�@     �&  :   5R	 @      6�@     �&  6�@     '  6�@     '  6@     5'  4Q@     ?'  �   5RO 4o@     W'  �   8R8Q 4�@     W'  �   8R8Q 6�@     s'   E�@     5R懶~  �  FWinMainCRTStartup �  �@     "       �4!  9ret �  �  6�@     �'  6�@     �   FmainCRTStartup �   @     "       湂!  9ret �  �  6@     �'  6@     �   :argc K  	(p@     :argv Q  	 p@     :envp R  	p@     :argret U  	p@     :mainret V  	p@     :managedapp W  	p@     :has_cctor X  	p@     :startinfo Y  	 p@     G__globallocalestatus L  G__imp__fmode --  G_dowildcard 4  G_newmode n  G__imp___initenv v^  G__imp__acmdln {  G__native_startup_state §"  )o  G__native_startup_lock �#  #  HG__image_base__ '�
  G__imp__commode 2-  G_fmode 1  �  Z#  I G__xi_a :O#  G__xi_z ;O#  �  �#  I G__xc_a <v#  G__xc_z =v#  G__dyn_tls_init_callback @�#  �  G__onexitbegin B�#  �  G__onexitend C�#  Gmingw_app_type E  J__mingw_winmain_hInstance GA  	z@     J__mingw_winmain_lpCmdLine H�   	z@     J__mingw_winmain_nShowCmd IA  	 0@     G__mingw_oldexcpt_handler Z{  Jmingw_pcinit f�  	 怈     Jmingw_pcppinit g�  	怈     G_MINGW_INSTALL_DEBUG_MATHERR i  Kmingw_initltsdrot_force W  Kmingw_initltsdyn_force X  Kmingw_initltssuo_force Y  Kmingw_initcharmax Z  L__set_app_type %     M_encode_pointer �%  �%  %   N_setargv �  L__mingw_setusermatherr 
拆%  �%   �%    &  &   z  M__getmainargs �  >&  -  ^  ^    >&     Mstrlen 6�   \&  �   Omalloc �%  u&  �    Pmemcpy %  �&  %  k  C   Q_pei386_runtime_relocator \N__mingw_init_ehandler �  M_set_invalid_parameter_handler �	  �&  	   Q_fpreset )Q__main LMmain �  5'         Q_cexit 7L_amsg_exit 哤'     L_initterm 6s'  �#  �#   Rexit 2�'     Q__security_init_cookie i �   3  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/mingw_helpers.c �@            1  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int double long double tagCOINITBASE 暣  COINITBASE_MULTITHREADED   VARENUM  H  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � _decode_pointer B  �@            湀  codedptr B  R _encode_pointer B  �@            溍  ptr B  R 	mingw_app_type �   	0p@      �   �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/dllargv.c �@              char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int double long double _setargv �   �@            � Y   �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/merr.c �@     \        double char long long unsigned int long long int short unsigned int int long int �   unsigned int sizetype long unsigned int unsigned char float signed char short int long double tagCOINITBASE 暞  COINITBASE_MULTITHREADED   VARENUM  C  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � _iobuf 0X�  _ptr Y   _cnt Z�   _base [  _flag \�   _file ]�   _charbuf ^�    _bufsiz _�   $_tmpfname `  ( 	FILE bC  _exception (�-  type ╊    name �-  arg1 珱   arg2 瑳   retval 瓫     3  
�   	fUserMathErr L  R  �   a  a   �  
__mingw_raise_matherr 靶@     A       滯  typ �     name -  ]  a1 �   �  a2 �   �  rslt �   � ex �  慇@     R慇  
__mingw_setusermatherr � @            淨  f L    ,@     
  R�R  _matherr �   0@     �       滊  pexcept %a  G  type '-  �  �@     (  �@     @  Q	XA@     Xs Yt w ��w(��w0��  stUserMathErr 8  	@p@     __setusermatherr �(  L   __iob_func T:  �  fprintf y�   :  -    �   k  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/pseudo-reloc.c 0@     �      �  __gnuc_va_list �   __builtin_va_list �   char va_list �   size_t #�   long long unsigned int long long int ptrdiff_t X  short unsigned int int long int �   unsigned int sizetype long unsigned int unsigned char BYTE 嫆  WORD �2  DWORD 峿  float PBYTE 愢  �  LPBYTE 戉  LPVOID 櫍    signed char short int ULONG_PTR 1�   SIZE_T �%  PVOID   double long double _MEMORY_BASIC_INFORMATION 0  	BaseAddress D   	AllocationBase D  	AllocationProtect �  	RegionSize 6  	State �   	Protect  �  $	Type !�  ( 
MEMORY_BASIC_INFORMATION "j  �  I  q   
;  PhysicalAddress <�  VirtualSize =�   _IMAGE_SECTION_HEADER (9�  	Name :9   	Misc >I  	VirtualAddress ?�  	SizeOfRawData @�  	PointerToRawData A�  	PointerToRelocations B�  	PointerToLinenumbers C�  	NumberOfRelocations D�   	NumberOfLinenumbers E�  "	Characteristics F�  $ 
PIMAGE_SECTION_HEADER G�    tagCOINITBASE 曢  COINITBASE_MULTITHREADED   VARENUM  }  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � _iobuf 0	  _ptr 	[   _cnt 	H  _base 	[  _flag 	H  _file 	H  _charbuf 	 H   _bufsiz 	!H  $_tmpfname 	"[  ( FILE 	$}  ;7  addend <�   target =�   runtime_pseudo_reloc_item_v1 >  F�  sym G�   target H�  flags I�   runtime_pseudo_reloc_item_v2 J[  L�  magic1 M�   magic2 N�  version O�   runtime_pseudo_reloc_v2 P�    〨	  old_protect    sec_start   hash 瓝     �	  __write_memory �	  addr �  src   len �    do_pseudo_reloc 3=
  start 3�  end 3�  base 3�  addr_imp 5!  reldata 5!  reloc_target 6!  v2_hdr 7=
  r 8C
  o hI
  newval m�     �  �  7  __report_error S0@     a       �  msg S  B  argp 捹   慩W@     �   r@     �  �
  !R	燗@     !Q1!XK |@     �   �@     
  �
  !Qs !Xt  �@     2     "�   #mark_section_writable �V  $addr 呆  %b �  %h 窊  %i 窰   &R	  �@     b      湨  'j	  x  'w	  �  '�	  �  (  �@     �   
�  '.  �  )�   *:  憪+C  y	  +L  �	   @     <  �  !Rs  3@     j  ,\@     
  !Q憪!X0 ,�@     !  !X@  �@     O
  @  !R	B@       �@     O
  _  !R	郃@      -@     O
  !R	繟@     !Qs    .�@     �  !R�R!Q�Q!X�X  #restore_modified_sections ��  %i 酘  %b �  %oldprot 憬   /_pei386_runtime_relocator �@     �      �  0was_init �H  	Pp@     1mSecs �H  3
  (�	  f@        ��  2�	  2�	  2�	  )   +�	  V
  *�	  憪+�	  h  +
  �  +
  �  (R	  �@     p  ��
  '�	  
  'w	  (
  'j	  K
  -�@     V  !Qu !X2  (R	  @@     �  �>  '�	  n
  'w	  �
  'j	  �
  -[@     V  !Qu !X8  3�@     =       �  + 
  �
  )   *+
  憪4R	  	@     0  p'�	  #  'w	  G  'j	  j  -#@     V  !Qu !X4    (R	  5@     `  �  '�	  �  'w	  �  'j	  �  -a@     V  !Qu !X1  (R	  �@     �  �O  '�	  �  'w	    'j	  >  -�@     V  !Qu !X4   �@     O
  n  !R	xB@      -	@     O
  !R	@B@        (�  j@     �  �  )�  +�  a  *�  憪*�  憣,�@     �  !Qu !X0 ,�@     �  !Y}  -�@     O
  !R	郃@        O@     �   the_secs �6  	Xp@     G	  maxSections 盚  	Tp@     5__RUNTIME_PSEUDO_RELOC_LIST__ 0�   5__RUNTIME_PSEUDO_RELOC_LIST_END__ 1�   5__image_base__ 2�   6__iob_func 	T�    7__builtin_fwrite 
 fwrite �   
  8  8�   8�   8�   9vfprintf 	}H  2  8�  8  8�    :abort @;__mingw_GetSectionForAddress   j  8�   6_GetPEImageBase в  <memcpy �  �  8�  8  8q   6__mingw_GetSectionCount    J   �	  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/crt_handler.c @     X        char size_t #�   long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char T  _  �    e  _EXCEPTION_RECORD ��	  ExceptionCode �	   ExceptionFlags �	  	B  �	_  ExceptionAddress �	�  NumberParameters �	  ExceptionInformation �	�	    
  _CONTEXT ��  P1Home �€   P2Home �€  P3Home �€  P4Home �€  P5Home �€   P6Home �€  (ContextFlags �  0MxCsr �  4SegCs �  8SegDs �  :SegEs �  <SegFs �  >SegGs �  @SegSs �  BEFlags �  DDr0 �€  HDr1 �€  PDr2 �€  XDr3 �€  `Dr6 �€  hDr7 �€  pRax �€  xRcx �€  €Rdx �€  �Rbx �€  �Rsp �€  �Rbp �€  �Rsi �€  �Rdi �€  �R8 �€  �R9 �€  �R10 �€  �R11 �€  �R12 �€  �R13 �€  �R14 �€  �R15 �€  �Rip �€  ��   
VectorRegister �#	   
VectorControl �€  �
DebugControl �€  �
LastBranchToRip �€  �
LastBranchFromRip �€  �
LastExceptionToRip �€  �
LastExceptionFromRip �€  � BYTE �=  WORD 屻   DWORD �(  float PBYTE �?    LPBYTE �?  signed char short int ULONG_PTR 1�   DWORD64 赂   PVOID �  LONG    LONGLONG ��   ULONGLONG ��   _M128A D�  Low E�   High F�   M128A G�  �       �  &       6    _ double long double _XMM_SAVE_AREA32  l�  ControlWord m   StatusWord n  TagWord o  Reserved1 p  ErrorOpcode q  ErrorOffset r  ErrorSelector s  Reserved2 t  DataOffset u  DataSelector v  Reserved3 w  MxCsr x  MxCsr_Mask y  FloatRegisters z   XmmRegisters {  �
Reserved4 |&  � XMM_SAVE_AREA32 }O  ���  Header ��   Legacy �   Xmm0 ��  �Xmm1 ��  �Xmm2 ��  �Xmm3 ��  �Xmm4 ��  �Xmm5 ��  �
Xmm6 ��   
Xmm7 ��  
Xmm8 ��   
Xmm9 ��  0
Xmm10 ��  @
Xmm11 ��  P
Xmm12 ��  `
Xmm13 ��  p
Xmm14 ��  €
Xmm15 ��  � �  �      �#	  FltSave ��  FloatSave ��  �   �  3	     PCONTEXT �  _RUNTIME_FUNCTION ��	  BeginAddress �   EndAddress �  UnwindData �   RUNTIME_FUNCTION �D	  o  �	     EXCEPTION_RECORD �	e  PEXCEPTION_RECORD �	�	  �	  _EXCEPTION_POINTERS �	8
  	B  �	�	   	(  �	3	   EXCEPTION_POINTERS �	 
   
    i
     ;�
  PhysicalAddress <  VirtualSize =   _IMAGE_SECTION_HEADER (9�  Name :Y
   Misc >i
  VirtualAddress ?  SizeOfRawData @  PointerToRawData A  PointerToRelocations B  PointerToLinenumbers C  NumberOfRelocations D   NumberOfLinenumbers E  "Characteristics F  $ PIMAGE_SECTION_HEADER G�  �
  �  �  �  S
   PTOP_LEVEL_EXCEPTION_FILTER �  LPTOP_LEVEL_EXCEPTION_FILTER �  tagCOINITBASE 昬  COINITBASE_MULTITHREADED   VARENUM  �  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � __p_sig_fn_t 	0N  _UNWIND_INFO �  VersionAndFlags    PrologSize   CountOfUnwindCodes   FrameRegisterAndOffset   AddressOfExceptionHandler     UNWIND_INFO !
  __mingw_SEH_error_handler ^�   @     �      湠  B  ^_  �  EstablisherFrame _  �  (  `  e  DispatcherContext a  B  action c�      6  dN     R  e�   �  !W@     �  �  "R;"Q0 #q@     �  "R; !�@     �  �  "R8"Q0 #�@     �  "R8 !�@     �  �  "R4"Q0 #@       "R4 !7@     �  $  "R8"Q1 $<@     �  !\@     �  M  "R8"Q0 !x@     �  i  "R8"Q1 !�@     �  �  "R4"Q1 %�@     �  "R;"Q1  __mingw_init_ehandler 3�   �@     �       渶  &was_here 5�   	坧@     e 6�   a  pSec 7�  �  _ImageBase 82  �  $�@     �  !�@     �  J  "R	癇@      !x@     -  b  "Rt  '�@     "R	纐@     "Xv   _gnu_exception_handler �   �@     �      �6  exception_data �6  +   6  腘  8  action �   �   R  迄   �  !
 @     �    "R;"Q0 ## @     -  "R; (Y @     B  "R�R !� @     �  ^  "R8"Q0 !� @     �  z  "R8"Q1 $� @     �  !� @     �  �  "R8"Q0 #� @     �  "R8 !!@     �  �  "R4"Q0 #!@     �  "R4 !1!@     �    "R8"Q1 !O!@     �    "R;"Q1 %c!@     �  "R4"Q1  8
  �	  L     &emu_pdata /<  	纐@     �  w     &emu_xdata 0g  	纏@     )__mingw_oldexcpt_handler �  	€p@     *signal 	<�  �  �   �   +_fpreset [,_GetPEImageBase &2  *_FindPESectionByName $�  "  "   (  -�   ._FindPESectionExec %�  �     �    �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/CRT_fp10.c p!@            �	  _fpreset 	p!@            � �     GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/gccmain.c €!@     �       p
  char long long unsigned int long long int ptrdiff_t X�   short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int double long double �  tagCOINITBASE 暷  COINITBASE_MULTITHREADED   VARENUM  X  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � func_ptr �  	__do_global_dtors €!@     5       湩  
p �  	�0@      X  	__do_global_ctors  �!@     U       �  nptrs "'  2  i #'  �  �!@     �  
R	€!@       __main 5 "@            淍  <"@     �   
initialized 2�   	@s@     X  h   __CTOR_LIST__ ]  __DTOR_LIST__ 
]  atexit T�   �    u   /
  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/gs_support.c @"@     �      �  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char double float long double g  _EXCEPTION_RECORD ��	  ExceptionCode �	   ExceptionFlags �	  \  �	a  ExceptionAddress �	�  NumberParameters �	  ExceptionInformation �	K
      _CONTEXT ��  P1Home �w   P2Home �w  P3Home �w  P4Home �w  P5Home �w   P6Home �w  (ContextFlags �  0MxCsr �  4SegCs �  8SegDs �  :SegEs �  <SegFs �  >SegGs �  @SegSs �  BEFlags �  DDr0 �w  HDr1 �w  PDr2 �w  XDr3 �w  `Dr6 �w  hDr7 �w  pRax �w  xRcx �w  €Rdx �w  �Rbx �w  �Rsp �w  �Rbp �w  �Rsi �w  �Rdi �w  �R8 �w  �R9 �w  �R10 �w  �R11 �w  �R12 �w  �R13 �w  �R14 �w  �R15 �w  �Rip �w  �	n	   
VectorRegister ��	   
VectorControl �w  �
DebugControl �w  �
LastBranchToRip �w  �
LastBranchFromRip �w  �
LastExceptionToRip �w  �
LastExceptionFromRip �w  � BYTE �.  WORD 屧   DWORD �  signed char short int UINT_PTR /�   ULONG_PTR 1�   ULONG64 俩   DWORD64 漏   PVOID �  LONG �   LONGLONG ��   ULONGLONG ��   
��  LowPart �   l  ��   
�  LowPart �   l  ��   _LARGE_INTEGER �L  �  u ��  QuadPart ��   LARGE_INTEGER �  _M128A D�  Low E�   High F�   M128A Gb  �  �  
   �  �  
     �  
  _ _XMM_SAVE_AREA32  l+  ControlWord m   StatusWord n  TagWord o  Reserved1 p  ErrorOpcode q  ErrorOffset r  ErrorSelector s  Reserved2 t  DataOffset u  DataSelector v  Reserved3 w  MxCsr x  MxCsr_Mask y  FloatRegisters z�   XmmRegisters {�  �
Reserved4 |�  � XMM_SAVE_AREA32 }�  ��^	  Header �^	   Legacy ��   Xmm0 ��  �Xmm1 ��  �Xmm2 ��  �Xmm3 ��  �Xmm4 ��  �Xmm5 ��  �
Xmm6 ��   
Xmm7 ��  
Xmm8 ��   
Xmm9 ��  0
Xmm10 ��  @
Xmm11 ��  P
Xmm12 ��  `
Xmm13 ��  p
Xmm14 ��  €
Xmm15 ��  � �  n	  
    ��	  FltSave �+  FloatSave �+  C   �  �	  
   CONTEXT �  PCONTEXT �  _RUNTIME_FUNCTION �+
  BeginAddress �   EndAddress �  UnwindData �   PRUNTIME_FUNCTION �E
  �	  W  [
  
   EXCEPTION_RECORD �	g  PEXCEPTION_RECORD �	�
  [
  _EXCEPTION_POINTERS �	�
  \  �	t
   ContextRecord �	�	   EXCEPTION_POINTERS �	�
  _FILETIME �1  dwLowDateTime �   dwHighDateTime �   FILETIME 狁
  NTSTATUS �  |  ft_scalar !�   ft_struct "1   FT #Q  __security_init_cookie 2@"@     �       �  cookie 4G  �  systime 5|  懓perfctr 6L  慇�"@       R懓 �"@     R慇  __report_gsfailure g #@     �       湕
  StackCookie gW  '  cookie i�
  慞controlPC kh  SimgBase kh  懜establisherFrame kh  慇fctEntry l+
  ]  hndData m�  慔:#@     �  R	€s@      Q#@     
  Rs QvhX0 �#@     C
  R0Xs w 	€s@     w(vxw0vpw80 �#@     V
  R0 �#@     q
  R	繠@      �#@     �
  Q増€€|   $@     n   G  �
  
   !�
  GS_ExceptionRecord %[
  	€x@     GS_ContextRecord &�	  	€s@     GS_ExceptionPointers (  	繠@     "�
  #__security_cookie ,G  	 1@     #__security_cookie_complement -G  	01@     $abort @ �      GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/tlssup.c  $@     �       �  char long long unsigned int long long int uintptr_t K�   short unsigned int int long int �   unsigned int sizetype long unsigned int unsigned char ULONG ,  WINBOOL �   BOOL 凎   DWORD �,  float LPVOID 橰  signed char short int ULONG_PTR 1�   PVOID 鶵  HANDLE �R  ULONGLONG ��   double long double   PIMAGE_TLS_CALLBACK 7  =  R  	�  	|  	�   
_IMAGE_TLS_DIRECTORY64 (  StartAddressOfRawData �   EndAddressOfRawData �  AddressOfIndex �  AddressOfCallBacks �  SizeOfZeroFill |   Characteristics |  $ IMAGE_TLS_DIRECTORY64 R  IMAGE_TLS_DIRECTORY *  _PVFV   __dyn_tls_init Rp  �  
u  R�  
�  R|  
€  R�  pfunc T�  ps U�    N  __dyn_tls_dtor 巔   $@     /       �  u  広  �  �  巪  �  €  帓  +  E$@     �   [  P$@     i       溈  v  d  �  �  �  z  �  �  €$@     &       �  v    �  >  �  b  €$@     &       �  �  �  �    �$@     �   __tlregdtor s�   �$@            滣  func sN  R __xd_a EN  	X怈     __xd_z FN  	`怈     _tls_index )T  	Ly@     _tls_start /
  	 燖     _tls_end 0
  	`燖     __xl_a 2  	8怈     __xl_z 3  	P怈     _tls_used 5�  	 燖     2  _CRT_MT M�   __dyn_tls_init_callback m  	蠦@       __xl_c n  	@怈     __xl_d �  	H怈     mingw_initltsdrot_force 橱   	Hy@     mingw_initltsdyn_force 戴   	Dy@     mingw_initltssuo_force 调   	@y@     __mingw_TLScallback a  	�  	|  	�    �   �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/cinitexe.c �
  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char _PVFV :  @  -  Q  �     __xi_a 
A  	怈     __xi_z A  	0怈     __xc_a A  	 怈     __xc_z 
A  	怈      +   �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/natstart.c *  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int double long double tagCOINITBASE 暆  COINITBASE_MULTITHREADED   VARENUM  1  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � �  漲  __uninitialized  __initializing __initialized  �  �1  __native_startup_state 
�  	 z@     	q  __native_startup_lock �  	(z@     
�  __native_dllmain_reason    	�0@     	�   __native_vcclrit_reason    	�0@      �    o  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/wildcard.c /  _dowildcard  �   	Py@     int  �    �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/_newmode.c �  _newmode �   	`y@     int  �    �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/xtxtmode.c �  _fmode �   	py@     int  �   �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/atonexit.c �$@     �       I  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int _onexit_t (u  {  �   double long double �  tagCOINITBASE 曇  	COINITBASE_MULTITHREADED   
VARENUM  f  	VT_EMPTY  	VT_NULL 	VT_I2 	VT_I4 	VT_R4 	VT_R8 	VT_CY 	VT_DATE 	VT_BSTR 	VT_DISPATCH 		VT_ERROR 
	VT_BOOL 	VT_VARIANT 	VT_UNKNOWN 
	VT_DECIMAL 	VT_I1 	VT_UI1 	VT_UI2 	VT_UI4 	VT_I8 	VT_UI8 	VT_INT 	VT_UINT 	VT_VOID 	VT_HRESULT 	VT_PTR 	VT_SAFEARRAY 	VT_CARRAY 	VT_USERDEFINED 	VT_LPSTR 	VT_LPWSTR 	VT_RECORD $	VT_INT_PTR %	VT_UINT_PTR &	VT_FILETIME � 	VT_BLOB � 	VT_STREAM � 	VT_STORAGE � 	VT_STREAMED_OBJECT � 	VT_STORED_OBJECT � 	VT_BLOB_OBJECT � 	VT_CF � 	VT_CLSID � 	VT_VERSIONED_STREAM � 	VT_BSTR_BLOB �	VT_VECTOR € 	VT_ARRAY €� 	VT_BYREF €€	VT_RESERVED €€	VT_ILLEGAL ��	VT_ILLEGALMASKED �	VT_TYPEMASK � _PVFV �  mingw_onexit !d  �$@     �       湇  func !d    
onexitbegin #�  慲
onexitend $�  慼retval %d  �  �$@     D  �$@     e  
  R8 %@     D  %@     D  -%@     x  K  Rs Q慲X慼 :%@     �  K%@     �  \%@     �  |  R8 q%@     Rs   f  atexit T�   €%@            溵  func 8f  �  �%@     s  R�R  __onexitbegin �  	0z@     __onexitend �  	8z@     d  )  d   __imp__onexit >    _decode_pointer �=  e  =   _lock x  �    __dllonexit d  �  d  �  �   �  _encode_pointer �=  �  =   _unlock �     �   h  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/charmax.c �%@     
       �  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int g  �   double long double tagCOINITBASE 暦  COINITBASE_MULTITHREADED   VARENUM  K  VT_EMPTY  VT_NULL VT_I2 VT_I4 VT_R4 VT_R8 VT_CY VT_DATE VT_BSTR VT_DISPATCH 	VT_ERROR 
VT_BOOL VT_VARIANT VT_UNKNOWN 
VT_DECIMAL VT_I1 VT_UI1 VT_UI2 VT_UI4 VT_I8 VT_UI8 VT_INT VT_UINT VT_VOID VT_HRESULT VT_PTR VT_SAFEARRAY VT_CARRAY VT_USERDEFINED VT_LPSTR VT_LPWSTR VT_RECORD $VT_INT_PTR %VT_UINT_PTR &VT_FILETIME � VT_BLOB � VT_STREAM � VT_STORAGE � VT_STREAMED_OBJECT � VT_STORED_OBJECT � VT_BLOB_OBJECT � VT_CF � VT_CLSID � VT_VERSIONED_STREAM � VT_BSTR_BLOB �VT_VECTOR € VT_ARRAY €� VT_BYREF €€VT_RESERVED €€VT_ILLEGAL ��VT_ILLEGALMASKED �VT_TYPEMASK � _PIFV a  	my_lconv_init �   �%@     
       湇  
�%@      mingw_initcharmax �   	€y@     _charmax �   	�0@     __mingw_pinit K  	(怈      n     GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/pesect.c �%@     �      �  char size_t #�   long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char WINBOOL �   BYTE �8  WORD 屴   DWORD �#  float PBYTE 悤  Z  LPVOID 橧  signed char short int ULONG_PTR 1�   DWORD_PTR 颗  LONG �   ULONGLONG ��   double long double Z  /     	_IMAGE_DOS_HEADER @��  
e_magic �f   
e_cblp �f  
e_cp �f  
e_crlc �f  
e_cparhdr �f  
e_minalloc �f  

e_maxalloc �f  
e_ss �f  
e_sp �f  
e_csum �f  
e_ip �f  
e_cs �f  
e_lfarlc �f  
e_ovno �f  
e_res ��  
e_oemid �f  $
e_oeminfo �f  &
e_res2 ��  (
e_lfanew ��  < f  �     f  �    	 IMAGE_DOS_HEADER �/  PIMAGE_DOS_HEADER ��  /  	_IMAGE_FILE_HEADER  �  
Machine !f   
NumberOfSections "f  �  #r  
PointerToSymbolTable $r  
NumberOfSymbols %r  
SizeOfOptionalHeader &f  �  'f   IMAGE_FILE_HEADER (�  	_IMAGE_DATA_DIRECTORY \�  �  ]r   
Size ^r   IMAGE_DATA_DIRECTORY _�  �       	_IMAGE_OPTIONAL_HEADER64 ��D  
Magic �f   
MajorLinkerVersion �Z  
MinorLinkerVersion �Z  
SizeOfCode �r  
SizeOfInitializedData �r  
SizeOfUninitializedData �r  
AddressOfEntryPoint �r  
BaseOfCode �r  
ImageBase ��  
SectionAlignment �r   
FileAlignment �r  $
MajorOperatingSystemVersion �f  (
MinorOperatingSystemVersion �f  *
MajorImageVersion �f  ,
MinorImageVersion �f  .
MajorSubsystemVersion �f  0
MinorSubsystemVersion �f  2
Win32VersionValue �r  4
SizeOfImage �r  8
SizeOfHeaders �r  <
CheckSum �r  @
Subsystem �f  D
DllCharacteristics �f  F
SizeOfStackReserve ��  H
SizeOfStackCommit ��  P
SizeOfHeapReserve ��  X
SizeOfHeapCommit ��  `
LoaderFlags �r  h
NumberOfRvaAndSizes �r  l
DataDirectory �  p IMAGE_OPTIONAL_HEADER64 �  PIMAGE_OPTIONAL_HEADER64 ��    PIMAGE_OPTIONAL_HEADER �d  _IMAGE_NT_HEADERS64 �	  
Signature �r   
FileHeader ��  
OptionalHeader �D   PIMAGE_NT_HEADERS64 �$	  �  PIMAGE_NT_HEADERS �	  
;z	  PhysicalAddress <r  VirtualSize =r   	_IMAGE_SECTION_HEADER (9w
  
Name :   
Misc >D	  �  ?r  
SizeOfRawData @r  
PointerToRawData Ar  
PointerToRelocations Br  
PointerToLinenumbers Cr  
NumberOfRelocations Df   
NumberOfLinenumbers Ef  "�  Fr  $ PIMAGE_SECTION_HEADER G�
  z	  
7�
  �  8r  OriginalFirstThunk 9r   	_IMAGE_IMPORT_DESCRIPTOR 6<  �
   �  ;r  
ForwarderChain =r  
Name >r  
FirstThunk ?r   IMAGE_IMPORT_DESCRIPTOR @�
  PIMAGE_IMPORT_DESCRIPTOR A}  <  _ValidateImageBase K  �  �  �  pDOSHeader �  �  *	  pOptHeader �   �  �%@            �  �  	   �  �  R�  r� �  �%@            淺  �  B   �  {   �  �  �%@     �   _FindPESection -w
  �%@     E       溡  �  -�  �   rva -�  Q�  /*	  �   �  0w
  4!  �  1  W!   _FindPESectionByName Cw
  @&@     �       溗
  pName C�
  {!  �  E�  �  F*	  "  �  Gw
  <"  �  H  r"   �  U&@       O�
  !�  "  �  �  �  #�&@     �  $Rs    %O&@     :  �
  $Rt  #�&@     R  $Rs $Qt $X8  �
  &�   __mingw_GetSectionForAddress bw
  �&@     C       渿  p b�  �"  �  d�  rva e�   �  �&@     P  hy  !�  "P  �  �  �  #'@     �  $Rs    #'@     \   __mingw_GetSectionCount p�   0'@     ;       �  �  r�  �  s*	  '�  5'@     €  v!�  "€  �  �  �  #X'@     �  $Rs     _FindPESectionExec 倃
  p'@     �       溚  eNo 偉   #  �  剤  �  �*	  �#  (�  唚
  Q�  �  �#  '�  w'@     �  �!�  "�  �  �  �  #�'@     �  $Rt     _GetPEImageBase 爤   (@     8       淗  �    '�  (@        �!�  "   �  �  �  #((@     �  $Rt     _IsNonwritableInCurrentImage 琄  @(@     U       �!  pTarget 瑘  �#  �  畧  rvaTarget   �  皐
  _$   �  F(@     @  �  !�  "@  �  �  �  #k(@     �  $Rs    #}(@     \  $Rs $Qt   __mingw_enum_import_library_names 浪
  �(@     �       �$  i 吏   �$  �  聢  �  �*	  )importDesc 腬  %  �  舧
  h%  )importsStartRVA 苧  �%   �  �(@     €  �	  !�  "€  �  �  �  #�(@     �  $Ru    #�(@     \  $Ru $Qs   *__image_base__ �  +strlen 6�   R  ,�
   -strncmp L�   ,�
  ,�
  ,�     �    T  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/pseudo-reloc-list.c �  __RUNTIME_PSEUDO_RELOC_LIST_END__ �   	憏@     char __RUNTIME_PSEUDO_RELOC_LIST__ �   	恲@      �   ~  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/tlsthrd.c P)@     '        char size_t #�   long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char WINBOOL �   WORD 屵   DWORD �$  float LPVOID 橨  signed char short int ULONG_PTR 1�   LONG �   HANDLE �J  _LIST_ENTRY ]  Flink ^   Blink _   �  LIST_ENTRY `�  double long double _RTL_CRITICAL_SECTION_DEBUG 0T2  Type U[   CreatorBackTraceIndex V[  CriticalSection W�  ProcessLocksList X  EntryCount Yg   ContentionCount Zg  $Flags [g  (CreatorBackTraceIndexHigh \[  ,SpareWORD ][  . _RTL_CRITICAL_SECTION (o�  DebugInfo p�   LockCount q�  RecursionCount r�  OwningThread s�  LockSemaphore t�  SpinCount u�    2  PRTL_CRITICAL_SECTION_DEBUG ^�  :  RTL_CRITICAL_SECTION v2  CRITICAL_SECTION �   ;  	F  
J   __mingwthr_key_t ^  __mingwthr_key �  key  g   dtor !5  next "�   �  
F  __mingwthr_run_key_dtors b�  keyp d�  value l}    �  P)@     m       渏  �  �%  €)@            7  �  �%  �)@     Rt   e)@     R  R	纘@      �)@     R	纘@       ___w64_mingwthr_add_key_dtor )�   �)@            �%  key )g  4&  dtor )5  �&  new_key +%  D'  �)@     `  �  R1QH *@     
  R	纘@      .*@     R	纘@       F  ___w64_mingwthr_remove_key_dtor @�   @*@     �       滪  key @g  �'  prev_key B�  �'  cur_key C�  ](  o*@     �  R	纘@      �*@     �  R	纘@      �*@     ~  �*@     R	纘@       __mingw_TLScallback yL  �*@     �       滪  hDllHandle y�  �(  reason zg  �)  reserved {}  �*  �  �*@     �  彇  �  �  e+@     �    �  +@     �  吰  �  �  u+@     �    0+@     �  R	纘@      ]+@     R	纘@        __mingwthr_cs   	纘@      __mingwthr_cs_init <  	▂@     
�    key_dtor_list &�  	爕@     !calloc �J  ~  
�   
�    "free �
J    �    3  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt/tlsmcrt.c y  _CRT_MT �   	�0@     int  �    ]  �  €+@     �+@     ../../../../../src/gcc-4.9.2/libgcc/config/i386/cygwin.S C:\crossdev\gccmaster\build-tdm64\gcc\x86_64-w64-mingw32\libgcc GNU AS 2.24.51 €�   q  GNU C 4.9.2 -mtune=generic -march=x86-64 -g -O2 -O2 -O2 -fbuilding-libgcc -fno-stack-protector ../../../../../src/gcc-4.9.2/libgcc/libgcc2.c C:\crossdev\gccmaster\build-tdm64\gcc\x86_64-w64-mingw32\libgcc Q  char long long unsigned int long long int short unsigned int int long int unsigned int sizetype long unsigned int unsigned char double float long double �  short int ix86_tune_indices I�  X86_TUNE_SCHEDULE  X86_TUNE_PARTIAL_REG_DEPENDENCY X86_TUNE_SSE_PARTIAL_REG_DEPENDENCY X86_TUNE_SSE_SPLIT_REGS X86_TUNE_PARTIAL_FLAG_REG_STALL X86_TUNE_MOVX X86_TUNE_MEMORY_MISMATCH_STALL X86_TUNE_FUSE_CMP_AND_BRANCH_32 X86_TUNE_FUSE_CMP_AND_BRANCH_64 X86_TUNE_FUSE_CMP_AND_BRANCH_SOFLAGS 	X86_TUNE_FUSE_ALU_AND_BRANCH 
X86_TUNE_REASSOC_INT_TO_PARALLEL X86_TUNE_REASSOC_FP_TO_PARALLEL X86_TUNE_ACCUMULATE_OUTGOING_ARGS 
X86_TUNE_PROLOGUE_USING_MOVE X86_TUNE_EPILOGUE_USING_MOVE X86_TUNE_USE_LEAVE X86_TUNE_PUSH_MEMORY X86_TUNE_SINGLE_PUSH X86_TUNE_DOUBLE_PUSH X86_TUNE_SINGLE_POP X86_TUNE_DOUBLE_POP X86_TUNE_PAD_SHORT_FUNCTION X86_TUNE_PAD_RETURNS X86_TUNE_FOUR_JUMP_LIMIT X86_TUNE_SOFTWARE_PREFETCHING_BENEFICIAL X86_TUNE_LCP_STALL X86_TUNE_READ_MODIFY X86_TUNE_USE_INCDEC X86_TUNE_INTEGER_DFMODE_MOVES X86_TUNE_OPT_AGU X86_TUNE_AVOID_LEA_FOR_ADDR X86_TUNE_SLOW_IMUL_IMM32_MEM  X86_TUNE_SLOW_IMUL_IMM8 !X86_TUNE_AVOID_MEM_OPND_FOR_CMOVE "X86_TUNE_SINGLE_STRINGOP #X86_TUNE_MISALIGNED_MOVE_STRING_PRO_EPILOGUES $X86_TUNE_USE_SAHF %X86_TUNE_USE_CLTD &X86_TUNE_USE_BT 'X86_TUNE_USE_HIMODE_FIOP (X86_TUNE_USE_SIMODE_FIOP )X86_TUNE_USE_FFREEP *X86_TUNE_EXT_80387_CONSTANTS +X86_TUNE_VECTORIZE_DOUBLE ,X86_TUNE_GENERAL_REGS_SSE_SPILL -X86_TUNE_SSE_UNALIGNED_LOAD_OPTIMAL .X86_TUNE_SSE_UNALIGNED_STORE_OPTIMAL /X86_TUNE_SSE_PACKED_SINGLE_INSN_OPTIMAL 0X86_TUNE_SSE_TYPELESS_STORES 1X86_TUNE_SSE_LOAD0_BY_PXOR 2X86_TUNE_INTER_UNIT_MOVES_TO_VEC 3X86_TUNE_INTER_UNIT_MOVES_FROM_VEC 4X86_TUNE_INTER_UNIT_CONVERSIONS 5X86_TUNE_SPLIT_MEM_OPND_FOR_FP_CONVERTS 6X86_TUNE_USE_VECTOR_FP_CONVERTS 7X86_TUNE_USE_VECTOR_CONVERTS 8X86_TUNE_AVX256_UNALIGNED_LOAD_OPTIMAL 9X86_TUNE_AVX256_UNALIGNED_STORE_OPTIMAL :X86_TUNE_AVX128_OPTIMAL ;X86_TUNE_DOUBLE_WITH_ADD <X86_TUNE_ALWAYS_FANCY_MATH_387 =X86_TUNE_UNROLL_STRLEN >X86_TUNE_SHIFT1 ?X86_TUNE_ZERO_EXTEND_WITH_AND � X86_TUNE_PROMOTE_HIMODE_IMUL � X86_TUNE_FAST_PREFIX � X86_TUNE_READ_MODIFY_WRITE � X86_TUNE_MOVE_M1_VIA_OR � X86_TUNE_NOT_UNPAIRABLE � X86_TUNE_PARTIAL_REG_STALL � X86_TUNE_PROMOTE_QIMODE � X86_TUNE_PROMOTE_HI_REGS � X86_TUNE_HIMODE_MATH � X86_TUNE_SPLIT_LONG_MOVES � X86_TUNE_USE_XCHGB � X86_TUNE_USE_MOV0 � X86_TUNE_NOT_VECTORMODE � X86_TUNE_AVOID_VECTOR_DECODE � X86_TUNE_AVOID_FALSE_DEP_FOR_BMI � X86_TUNE_BRANCH_PREDICTION_HINTS � X86_TUNE_QIMODE_MATH � X86_TUNE_PROMOTE_QI_REGS � X86_TUNE_ADJUST_UNROLL � X86_TUNE_LAST �  ix86_arch_indices �  X86_ARCH_CMOV  X86_ARCH_CMPXCHG X86_ARCH_CMPXCHG8B X86_ARCH_XADD X86_ARCH_BSWAP X86_ARCH_LAST  signed char __int128 __int128 unsigned complex float complex double  complex long double __float128  __unknown__ func_ptr *�  �  �  	J   
__CTOR_LIST__ 	�  	€-@     
__DTOR_LIST__ 	�  	�-@      �    �  GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/misc/mingw_matherr.c �  _MINGW_INSTALL_DEBUG_MATHERR �   	 1@     int  \     GNU C 4.9.2 -m64 -mtune=generic -march=x86-64 -g -O2 -std=gnu99 C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/misc/invalid_parameter_handler.c �+@            S  char long long unsigned int long long int uintptr_t K�   wchar_t b  short unsigned int int long int unsigned int sizetype long unsigned int unsigned char float signed char short int double long double �  �  �  �  �  -  �    �  �   	tagCOINITBASE �  
COINITBASE_MULTITHREADED   VARENUM  �  
VT_EMPTY  
VT_NULL 
VT_I2 
VT_I4 
VT_R4 
VT_R8 
VT_CY 
VT_DATE 
VT_BSTR 
VT_DISPATCH 	
VT_ERROR 

VT_BOOL 
VT_VARIANT 
VT_UNKNOWN 
VT_DECIMAL 
VT_I1 
VT_UI1 
VT_UI2 
VT_UI4 
VT_I8 
VT_UI8 
VT_INT 
VT_UINT 
VT_VOID 
VT_HRESULT 
VT_PTR 
VT_SAFEARRAY 
VT_CARRAY 
VT_USERDEFINED 
VT_LPSTR 
VT_LPWSTR 
VT_RECORD $
VT_INT_PTR %
VT_UINT_PTR &
VT_FILETIME � 
VT_BLOB � 
VT_STREAM � 
VT_STORAGE � 
VT_STREAMED_OBJECT � 
VT_STORED_OBJECT � 
VT_BLOB_OBJECT � 
VT_CF � 
VT_CLSID � 
VT_VERSIONED_STREAM � 
VT_BSTR_BLOB �
VT_VECTOR € 
VT_ARRAY €� 
VT_BYREF €€
VT_RESERVED €€
VT_ILLEGAL ��
VT_ILLEGALMASKED �
VT_TYPEMASK � _InterlockedExchangePointer �o  �  
Target ��  
Value �o   �  o  mingw_get_invalid_parameter_handler �  �+@            �mingw_set_invalid_parameter_handler �  �+@            満  new_handler �  R�  �+@     
       �  R�  
 z@     �  handler �  	 z@     �  �  �   __imp__set_invalid_parameter_handler   	1@     �  �  __imp__get_invalid_parameter_handler Y  	1@                                                                                                                                                                                                                                                                                                                                                                                                              %  $ >   :;I   I  :;  
 :;I8  
 :;I8     	:;  

 I8  
 :;I8  &   
 :;I  'I   I  I  ! I/   'I   '  '  & I  :;  :;  
 :;I  
 I  :;  
 I8  :;  
 :;I8  :;  ( 
   :;  !:;  ":;  # :;I  $.?:;'I   % :;I  &4 :;I  ' <  (. ?:;'I   )5 I  *.:;'   +  ,.:;'@桞  - :;I  ..:;'I   /.:;'I@桞  01RUXY  1U  24 1  34 1  4墏1  5妭 態  6墏 1  7墏1  8妭   94 :;I  :4 :;I  ;U  <1XY  =1XY  > 1  ?  @ 1  A1RUXY  B  C1XY  D墏  E墏  F.?:;'I@桞  G4 :;I?<  H5   I!   J4 :;I?  K4 :;I?<  L.?:;'<  M.?:;'I<  N. ?:;'I<  O.?:;'I<  P.?'I4<  Q. ?:;'<  R.?:;'<   %  $ >     :;  ( 
  :;  .?:;'I@桞   :;I  	4 :;I?   %  $ >  . ?:;'I@桞   %  $ >   I  :;  ( 
  :;  :;  
 :;I8  	 :;I  
& I  'I   I  
.?:;'@桞   :;I   :;I  4 :;I  墏  妭 態  墏旴1  .?:;'I@桞  4 :;I  墏 1  墏1  .?:;'<  . ?:;'I<  .?:;'I<      %   :;I   I  $ >   I     &   :;  	
 :;I8  
 :;I  I  ! I/  
:;  
 :;I  :;  ( 
  :;  :;  
 :;I8  :;  :;   :;I  .:;'    :;I  4 :;I    .:;'@桞   :;I     4 :;I  墏 1   墏1  !妭 態  "& I  #.:;'   $ :;I  %4 :;I  &.1@朆  ' 1  (1RUXY  )U  *4 1  +4 1  ,墏  -墏1  .墏旴1  /.?:;'@桞  04 :;I  14 :;I  2 1  3  41RUXY  54 :;I?<  6. ?:;'I<  7.?:;n'I<  8 I  9.?:;'I<  :. ?:;'<  ;.?:;'I<  <.?'I4<   %  $ >   :;I   I  '   I  :;  
 :;I8  	
 :;I8  
   :;  
 I8  

 :;I8   :;I  I  ! I/  :;  :;  
 :;I  
 I  :;  'I  :;  ( 
  :;  :;  
 :;I8  .?:;'I@桞   :;I   :;I  4 :;I   4 :;I  !墏1  "妭 態  #墏  $墏 1  %墏1  &4 :;I  '墏  (墏旴  )4 :;I?  *.?:;'I<  +. ?:;'<  ,. ?:;'I<  -& I  ..?:;'I<   %  . ?:;'@桞   %  $ >   :;I   I   '  :;  ( 
  :;  	.?:;'@朆  
4 :;I  4 :;I  墏旴1  
妭 態  .?:;'@桞  墏 旴1  I  !   4 :;I?<  .?:;'I<   I   %  $ >   I  :;  
 :;I8  
 :;I8     :;  	
 I8  

 :;I8   :;I   :;I  
:;  :;  
 I  
 :;I  I  ! I/  :;  :;  :;  
 :;I8  :;  
 :;I  .?:;'@朆  4 :;I  4 :;I  墏  妭 態  墏   :;I   墏 1  !5 I  "& I  #4 :;I?  $. ?:;'<   %  $ >   :;I   I      :;I   '  '  	 I  
:;  
 :;I8  .?:;'I   
 :;I  4 :;I  .:;'I@桞   :;I  墏 1  .1@朆   1  4 1      4 1  .?:;'I@桞   :;I  4 :;I  4 :;I?  & I  4 :;I?<  .?:;'I<   %  $ >   :;I   I   '  I  ! I/  4 :;I?   %  $ >  :;  ( 
  :;  :;   :;I  4 :;I?  	5 I  
 I  5    %  4 :;I?  $ >   %  4 :;I?  $ >   %  4 :;I?  $ >   %  $ >      :;I   I   'I   '  :;  	( 
  
:;  .?:;'I@桞   :;I  
4 :;I  4 :;I  墏 1  墏1  妭 態  墏  .?:;'I@桞  墏1  4 :;I?  'I   I  4 :;I?<  .?:;'I<  .?:;'<  .?:;'<   %  $ >   I   'I  :;  ( 
  :;   :;I  	.:;'I@桞  
墏 旴  4 :;I?   %  $ >   :;I      I   :;I  I  ! I/  	:;  

 :;I8  
 :;I8  :;  
:;  
 :;I  
 :;I  
 I8  .?:;'I    :;I  4 :;I  4 :;I  .1@桞   1  4 1  4 1  4 1  墏 旴1  .?:;'I@桞   :;I   :;I  4 :;I   :;I   1RUXY  ! 1  "U  #墏1  $妭 態  %墏1  && I  '1RUXY  (4 :;I  )4 :;I  *4 :;I?<  +.?:;'I<  , I  -.?:;'I<   %  4 :;I?  $ >   %  $ >   :;I      :;I  :;  
 :;I8   I  	'  
 I  :;  
 :;I8  
5 I  .:;'   4 :;I    .1@朆  4 1    墏  妭 態  墏  墏旴  .?:;'I@桞   :;I  4 :;I  墏1  墏 1  1RUXY  U  4 1   4 :;I  !.?:;'I<  ".?:;'<   %  4 :;I?  $ >    %   %  $ >   I   '  :;  ( 
   :;I  I  	! I/  
4 :;I?   %  4 :;I?  $ >   %  $ >   :;I      I  '   I  & I  	:;  
( 
  :;  .?:;'I   
 :;I  5 I  . :;'I@桞  .:;'I@桞   :;I  1XY   1  4 :;I  'I  4 :;I?   'I                                                                                                                                                                                                                                                                                                                                                                                                                                                                       -   �  �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include/psdk_inc C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/include  crtexe.c   intrin-impl.h   crtdefs.h   winnt.h   minwindef.h   basetsd.h   stdlib.h   errhandlingapi.h   processthreadsapi.h   combaseapi.h   wtypes.h   internal.h   math.h   tchar.h   ctype.h   string.h   process.h     	 @     � 	��J{y.g缮�杶謘gg��PZ	�lt�謚�`�攡惲�t�#KyE��;t�st
�sJ
f��飢��J媧.xt1���儂�
Xut.挨牪呭糦hZ糧鬯 ��uJ
菿I厀� �i� t?9>u"缮I=稏��t_譭g0dh厽� k�@鋙]�
灝2+i��鮵�蛘g����軂濵蒠Z2	.M蒠Z2 �    �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  mingw_helpers.c   combaseapi.h   wtypes.h     	�@     >�> m    S   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  dllargv.c     	�@      Q     �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/include  merr.c   combaseapi.h   wtypes.h   internal.h   math.h   stdio.h     	�@     L缹OTYggg=Av�[��Ce忽u恥媢媢恥 )     �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  pseudo-reloc.c   vadefs.h   crtdefs.h   minwindef.h   basetsd.h   winnt.h   combaseapi.h   wtypes.h   stdio.h   <built-in>    stdlib.h     	0@     � ?fAX? Z��� J��� t��Z�9蝺;=kg儬d>VLmuW=Z$� t	�w��Xteu�w����nJ燳趡f��=�趡簄fg�
怣f篖!<b濧	.wJ7<x<D 然N 邁渚{u.0��� $A=∕=z�.瀃h�=g瀐=y瀞�sX[�=祣瀝uI��
 �   
  �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  crt_handler.c   winnt.h   crtdefs.h   minwindef.h   basetsd.h   errhandlingapi.h   combaseapi.h   wtypes.h   signal.h     	@     � O7互搖?.��4只I=5@<� X1u.��只u\阂� XhX鎽.e只I=h�Y�<XNX�kt�C �[s�瀁h浠|zt+gDx<�>OF�>�z< f�/� 鋇�0互[� ����  �>V��2f籬鏅籱[�f�X籱�T�< n    T   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  CRT_fp10.c     	p!@     	    �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  gccmain.c   combaseapi.h   wtypes.h   crtdefs.h   stdlib.h     	€!@     M�/㎝q]鰃�91 3 � V]uex� .�簟 0   �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  gs_support.c   winnt.h   minwindef.h   basetsd.h   stdlib.h     	@"@     2｝揨 �d犬^儍hV0�.u.2m?畊uX濩蓇�;=h6
tw.y�y瀨g�=jX� 	   �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  tlssup.c   crtdefs.h   minwindef.h   basetsd.h   winnt.h     	 $@     �P�,Z�琸籬ZXx謚絻*�s�� Z    T   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  cinitexe.c       �   �
      C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/include  combaseapi.h   wtypes.h   natstart.c   internal.h    Z    T   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  wildcard.c    Z    T   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  _newmode.c    Z    T   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  xtxtmode.c    A     �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/include  atonexit.c   combaseapi.h   wtypes.h   stdlib.h   internal.h     	�$@     !XA糎Z0熁sYZ芞ZV>YWuYWuY=uf鸿K�    �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/include  charmax.c   combaseapi.h   wtypes.h   internal.h     	�%@     u �   �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  pesect.c   crtdefs.h   minwindef.h   basetsd.h   winnt.h   string.h     	�%@     	v-�+�q.	t�v慖龌H`/蝨CT�-tS.=tp�.sXMv慖"H\K?��f� �aM=;>e%�X� t�.� tz.MLY��t� tx.�<� to.MwI唄ZzJHT.�f�t鷡.�tg&騸f�t雫.�tyJL=;=YZ�zt
�迆��tw.迆.�tc萂LuM蒤
.=zJ twKld. c    ]   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  pseudo-reloc-list.c    t   �   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  tlsthrd.c   crtdefs.h   minwindef.h   basetsd.h   winnt.h   minwinbase.h   stdlib.h     	P)@     � 埵爑0憭xJX讈�fw�r<�;=1/L蕎rsMg/t�俓�i.藇Z.sJX�u鋭_u銰?*濳i�-�S� 灟s�w��oX�nX' Y    S   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/crt  tlsmcrt.c    w    O   �
      ../../../../../src/gcc-4.9.2/libgcc/config/i386  cygwin.S     	€+@     �""gY0uKgg0=L"" �    �   �
      ../../../../../src/gcc-4.9.2/libgcc/../gcc/config/i386 ../../../../../src/gcc-4.9.2/libgcc  i386.h   libgcc2.c   gbl-ctors.h    `    Z   �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/misc  mingw_matherr.c    ^   5  �
      C:/crossdev/src/mingw-w64-v3-git/mingw-w64-crt/misc C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include/psdk_inc C:/crossdev/gccmaster/host-toolchain-tdm64/x86_64-w64-mingw32/include  invalid_parameter_handler.c   intrin-impl.h   crtdefs.h   combaseapi.h   wtypes.h     	�+@     v��峹�                                                                               ���� x �              @            ,       @     A      D0�
Ap
A              `@     N       D@I \       �@           B�B�A �A(�A0�A8�G�Y
8A�0A�(A� A�B�B�A             �@     "       D0]          @     "       D0]     ���� x �            �@                  �@               ���� x �         h  �@               ���� x �         �  �@     A       D`|     �   @            <   �  0@     �       A�A�D�P�
���
儇�A�A�E    ���� x �      $   (  0@     a       A�A�DP   \   (  �@     b      B�B�A �A(�A0�A8�D�
8A�0A�(A� A�B�B�E      T   (  @     �      A�B�B �B(�A0�A8�A@�C
R
肁腁臖藼虰蜛�8H         ���� x �      <      @     �      D0c
IK
EW
A^
Jy
A    L      �@     �       B�A�A �A(�A0�DP{
0A�(A� A�A�B�E  4      �@     �      A�D0w
A�Ce
A�J      ���� x �            p!@               ���� x �         0  €!@     5       D0p  4   0  �!@     U       A�A�D@o
A�A�I        0   "@               ���� x �      l   �  @"@     �       B�A�A �A(�A0�Dpm
0A�(A� A�A�B�Cu
0A�(A� A�A�B�E       ,   �   #@     �       A�A�A �C
         ���� x �      $   p   $@     /       D0R
JN    D   p  P$@     i       A�A�D@e
A�A�Co
A�A�A          p  �$@               ���� x �      4     �$@     �       A�D@�
A�APA�           €%@            D0T     ���� x �         €  �%@     
          ���� x �         �  �%@               �  �%@               �  �%@     E       \   �  @&@     �       A�A�A �D@e
 A�A�A�AI
 A�A�A�DZ A�A�A�  <   �  �&@     C       A�A�D@T
A�A�D\A�A�  4   �  0'@     ;       A�D0T
A�FYA�       \   �  p'@     �       A�A�A �D@Z
 A�A�A�AQ
 A�A�A�GJ A�A�A�  <   �   (@     8       A�A�D@W
A�A�AUA�A�  D   �  @(@     U       A�A�D@T
A�A�Dn
A�A�A       l   �  �(@     �       A�A�A �A(�DPY
(A� A�A�A�Kf
(A� A�A�A�FI(A� A�A�A�    ���� x �      D   @	  P)@     m       A�A�A �A(�DP^(A� A�A�A�     L   @	  �)@            A�A�A �D@N
 A�A�A�HT
 A�A�A�A 4   @	  @*@     �       A�D0P
A�JJ
A�E   $   @	  �*@     �       D0_
AK
A   ���� x �      ,   P
  €+@     2       A�A€n繟�         ���� x �         �
  �+@               �
  �+@                                                                                                                                                                                                                                                                                                            Subsystem CheckSum SizeOfImage BaseOfCode SectionAlignment MinorSubsystemVersion DataDirectory SizeOfStackCommit ImageBase SizeOfCode MajorLinkerVersion SizeOfHeapReserve SizeOfInitializedData SizeOfStackReserve SizeOfHeapCommit MinorLinkerVersion __enative_startup_state SizeOfUninitializedData AddressOfEntryPoint MajorSubsystemVersion SizeOfHeaders MajorOperatingSystemVersion FileAlignment NumberOfRvaAndSizes ExceptionRecord DllCharacteristics MinorImageVersion MinorOperatingSystemVersion LoaderFlags Win32VersionValue MajorImageVersion sSecInfo ContextRecord old_handler ExceptionRecord reset_fpu ExceptionRecord HighPart hDllHandle lpreserved dwReason __enative_startup_state pSection TimeDateStamp pNTHeader Characteristics pImageBase VirtualAddress iSection                                                                                                                                                                                                                                                                 �              P3      Q       P                �              p�3      Q       p�                �              p�                �      �       0熍      *       P+      �       0�                �      �       0熚      �       R�      �       R�      �       x 1'�+      �       0�                �             0�                �      )       T+      7       T                �      +       0�+      ^       U!      +       U+      <       0�<      �       U�      �       U                �      �       0煘      �       0�                �             P                      �       0煘      �       0�                      )       T+      7       T                =      !      
  p@     熅      �      
  p@     �                =             \�      �       \                ^             U�      �       U                ^      f       0焒      �       S�      �       0�                W      f       Pf             V�      �       P�      �       V                }      �       T                €      �       0�                �      �       �熾      �       P                             ��      "       P                        "        R"       ;        r ;       A        �R�                        ;        Q;       A        �Q�                        ;        c;       A        ����                        ;        d;       A        ����                P       [        R[       \        �R�                `       �        R�       �        �R燑       \       R                �       �        S            
 燖@     �      ,      
 A@     �,      <      
 鐯@     �<      L      
 繞@     烲      \      
 FA@     �                        &        R&       a        S                p       �        R�       ~       S~      �       R�      �       �R煀      �       R�      �       S                p       �        Q�              T      �       Q�      �       �Q煀      �       Q�      �       T                p       �        X�       €       U€      �       X�      �       �X煀      �       U                �       �        R�       ~       S~      �       R�      �       �R煀      �       R�      �       S                �              P      p       \�      �       \�      �       P�      �       \                �       �        0煝       �        Y�      �       0�                      %       P                �      �       P�             X             p       &      
 s �����| "�      %       X%      ,      
 s �����| "6      Z       XZ      a      
 s �����| "k      ~       X      �       X�      �      
 s �����| "                c      �       P�      *       R�      �       P�      �       s�����| "熸      �       s|�����| "熼      �       R�      0       R6      e       Rk      r       Rr      ~       s�����| "�      �       R�      �       P                �      �       S�      �       st熖      �       S                �      4       S�      �       S                a      k       2�                a      k       ]                a      e       R                &      +       8�                &      +       ]                &      *       R                �      �       S�      �       sx燇      �       S                �      �       4�                �      �       U                �      �       R                ,      6       1�                ,      6       ]                ,      0       R                �      �       4�                �      �       ]                �      �       R                :      Z       0焃      �       T�      �       T                        B        RB       h        �R焗       �        R�       �        �R熂       �        R�       �        �R燐              R      7       �R�7      G       RG      �       �R�                        =        Q=       h        �Q焗       �        Q�       �        �Q熂       �        Q�       �        �Q燐              Q      7       �Q�7      B       QB      �       �Q�                        F        XF       h        �X焗       �        X�       �        �X熂       �        X�       �        �X燐              X      7       �X�7      K       XK      �       �X�                        F        YF       h        �Y焗       �        Y�       �        �Y熂       �        Y�       �        �Y燐              Y      7       �Y�7      K       YK      �       �Y�                       a        1焌       c        0焗       �        1煹       �        0熂       �        1燈       �        0燐       j       1焜      o       Po      �       1焿      �       0煇      �       1煙      �       0�                G       `        P�       �        P�       �        Q�       �        P      &       PS      g       Po      u       Pz      �       P�      �       P                       c        0焗       �        0煆       �        1熂              0�      ,       1�7      �       0�                �      �       0燑      2       0�2      �       T                2      M       Ph      x       P                �      �       P�      �       V�      �       V                �      �       R�             S             �R�      2       R2      F       SF      H       RH      I       �R烮      n       Rn      �       S�      �       R�      �       S�      �       R�      X       S                �             Ps      �       P�      �       P�             P              P&      >       PD      R       P                �             0�             	��      �       0煱      �       	�熀             0�             	��      ?       0�?      D       	�烡      S       0烻      X       	��                �             0�      2       0烮      g       0焔      �       1煉      �       0熀      X       0�                U       W        PW       [        S|       �        P�       �        S                W       e        Se       h        s焗       v        S                P       X        \X       `        v ����| '焋       e        v ����p ����'| '焑       m        v ����u ����'| '焟       r        v ����u ����'p ����'| '焤       x        v ����u ����'t ����'| '煏       �        T�       �       	  1@     �       �        T�       �        u ������?�                �       �        R�       �       T                      !       P!      R       Y�      �       P�      �       Y                        $        R$       /        �R�                        $        Q$       /        �Q�                        $        X$       /        �X�                0       R        RR       ^        �R焇       s        Rs       �        �R煉       �        R�       �        �R�                0       R        QR       ^        �Q焇       s        Qs       �        �Q煉       �        Q�       �        �Q�                0       R        XR       ^        �X焇       s        Xs       �        �X煉       �        X�       �        �X�                ^       s        Rs       �        �R�                ^       �        2�                ^       s        Xs       �        �X�                s       �        S�       �        sx�                ^       g       
 X怈     焔       �        S                                R       e        Se       �        �R煏       �        S�       �        �R�                e       i        Pi       �        S�       �        P                �       �        R�       �        �R�                                R               �R�                        1        R1       2        �R�                        1        R1       2        �R�                @       G        RG       �        �R�                G       T        RT       �        �R#<� $ &�R"�                P       �        P                G       d        0�                �       �        R�       �        T�       �        �R熇       �        T�       �        �R熗       ,       T,      .       �R�                �       �        Q                �       +       S+      .       P                �              0�                0      D       RD      M       �R烳      V       RV      b       Tb      s       �R�                �      �       R�      �       S�      �       �R熷      �       R�      7       S:      K       S                �      8       T:      L       T                �             0�                �      �       R�      �       �R煭      �       R�      �       T�      �       �R�                �      �       P�      �       P                �      
       R
             T             �R�      #       R#      �       T�      �       T                M      P       u s "烶      W       SW      �       Q                H      W       P                7      C       QC      G       u p "#�G      P       S                       g        S                7       8        P8       T        T                p       �        R�       �        �R焿       �        R�       �        U�       �        �R熻       �        U                p       �        Q�       �        �Q焿       �        Q�       �        T�       �        �Q熻       �        T                �       �        P�       �        S�       �        P�       �        S                �              R      [       S[      \       �R焅      �       S                      3       0�3      G       Q\      k       Q      �       0�                &      3       Q3      >       P>      B       QB      G       P\      g       Pg      o       R      �       Q                �      �       R�      �       �R煷      �       R�      �       �R熲      �       R�              �R�              R             �R�             R             �R�      $       R$      '       �R�                �      �       Q�      �       �Q煷      �       Q�      �       �Q熲      �       Q�              �Q�              Q             �Q�             Q             �Q�      $       Q$      '       �Q�                �      �       X�      �       �X煷      �       X�      �       �X熲      �       X�              �X�              X             �X�             X             �X�      $       X$      '       �X�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   X       �              3      Q                      �            !      �      �      �                      �      �                                  =      �      �      �                      |       �       �       p      �      �                      6      D      ]      :      �      �      �      �                      �      �      9      <      T      ]      a      k                                        "      &      +                      �      �      �      �                      �      �      �      �                                        (      ,      6                      �      �      �      �      �      �                      :      �      �      �                      �       �       �       �       �       �                       6      D      S      [                      �      �      �      �      �      �                      �      �      �      �      �      �                      V      ]      _      f      p      x                      �      �      �      �      �      �                      �      �            
             (                      �      �                                   �      �             '                                                                                                                                                                                                                                                      .file   g   ��  gcrtexe.c              j                                �              �   �                        �   �                        �   �                          €                        )  @                        G  p                        e             p  �                        �  �                        �  0                        �  p                        �  �                          `            `                    envp           argv            argc    (           +              5                       argret             P  �          b  �                        �  �                        �             �  0                        �  �                          `                        #  P                    mainret            B  �                        X  �                        n                          �                           �  �          �  �      .l_endw �          �         .l_start      .l_end            �  @                        �  P                    .text          "  N             .data                           .bss           ,                 .xdata         h   
             .pdata         H                                                
                                      �'  �                 #         3                    1                            <      
   0                    K         �                     Y      
   1                   e                             p  �                          {                         .file   q   ��  gcrtbegin.c        .text   0                       .data                           .bss    0                            p  �                      .file   �   ��  gstruct5.c         main    0                       average `      .text   0     e  	             .data   @      x                .bss    0                        .rdata         `                 .xdata  h                       .pdata  H                          p                         .file   �   ��  gmingw_helpers.c       �  �                           �  �      .text   �                      .data   �                        .bss    0                       .xdata  €                       .pdata  `                            �'     �                   #  3     �                     <  0   
   0                    Y  1  
   �                    p                             {        H                .file   �   ��  gdllargv.c         _setargv�                       .text   �                      .data   �                        .bss    @                        .xdata  �                       .pdata  x                            �,     �                   #  �     7                     <  `   
   0                    Y    
   q                    p  @                          {  h     0                .file   �   ��  gmerr.c                �  �                           �  @           �         _matherr0      .text   �     \               .data   �                        .bss    @                       .xdata  �      $                 .pdata  �      $   	             .rdata  `      @                     ".     ]                   #  �     z                    1       2                   <  �   
   0                    Y    
   U                   p  `                          {  �     �                .file     ��  gpseudo-reloc.c        �  0                           �  �            T       the_secsX             
          .  P           =                          n                       .text   0     �  *             .data   �                        .bss    P                       .rdata  �                     .xdata  �      8                 .pdata  �      $   	                   5     �  \                 #  k     "                    1  B     i                    <  �   
   0                    K  �      0                    Y  �  
   -                   e       	                     p  €                          {  (     �                .file   *  ��  gcrt_handler.c         �  
                           �  �          �  �           �  �          �  �           �  �      .text   
     X               .data   �                        .bss    €      �                .xdata  �                        .pdata  �      $   	             .rdata  �                            @G     N  @                 #  �	     d                    1  �     �	                    <  �   
   0                    Y    
   �                   e  (     4                     p  �                          {        �                .file   E  ��  gCRT_fp10.c        _fpresetp                       fpreset p      .text   p                      .data   �                        .bss    @                       .xdata                        .pdata  �                            嶾     �                    #  �     *                     <     
   0                    Y  �	  
   r                    p  �                          {        0                .file   g  ��  ggccmain.c               €                       p.73208 �             �          )                       __main             F  @      .text   €     �                .data   �                      .bss    @                      .xdata                        .pdata  �      $   	                   L]     �  
                 #                           1  2     �                     <  P  
   0                    Y  p
  
                      p  �                          {  0     �                .file   �  ��  ggs_support.c          R  @                           i                            �  0                         �             �  €          �  €          �  �      .text   @     �               .data   �                        .bss    €     �                .xdata  $                       .pdata                        .rdata  �                           鬮     y                   #  /
     �                    1  �     �                   <  €  
   0                    Y  �  
   4                   e  \                          p                             {  �     �                .file   �  ��  gtlssup.c              �                              �  P            �                    __xd_a  X       __xd_z  `           %  �      .text         �                .data   �                        .bss    @	                      .xdata  D                      .pdata  8     $   	             .CRT$XLDH                      .CRT$XLC@                      .rdata  �                     .CRT$XDZ`                       .CRT$XDAX                       .tls        	   (                .CRT$XLZP                       .CRT$XLA8                       .tls$ZZZ`   	                    .tls$AAA    	                          mq     �  )                 #        �                    1  �     Y                   <  �  
   0                    Y  �  
   
                   e  u                          p                             {  p     �                .file   �  ��  gcinitexe.c        .text   �                       .data   �                        .bss    P	                       .CRT$XCZ                       .CRT$XCA                        .CRT$XIZ0                       .CRT$XIA                             Hx     �                   #  �     Y                     <  �  
                        Y  �
  
   ^                     p  @                      .file   �  ��  gnatstart.c        .text   �                       .data   �                       .bss    P	                             鷜     /                   #  �     �                     <     
                        Y  *  
                       e  �                          p  `                      .file     ��  gwildcard.c        .text   �                       .data   �                        .bss    P	                            )     �                    #  o     *                     <     
                        Y  /  
   ^                     p  €                      .file     ��  g_newmode.c        .text   �                       .data   �                        .bss    `	                            �     �                    #  �     *                     <  @  
                        Y  �  
   ^                     p  �                      .file   +  ��  gxtxtmode.c        .text   �                       .data   �                        .bss    p	                            巰     �                    #  �     *                     <  `  
                        Y  �  
   ^                     p  �                      .file   J  ��  gatonexit.c            1  �                           >  €                    atexit  €      .text   �     �                .data   �                        .bss    €	                       .xdata  \                      .pdata  \                           =�     �                   #  �     {                    1       �                     <  €  
   0                    Y  I  
   E                   p  �                          {       p                .file   f  ��  gcharmax.c             [  �                       .text   �     
                .data   �                       .bss    €	                      .xdata  l                      .pdata  t                     .CRT$XIC(                            �     �                   #  h     �                     <  �  
   0                    Y  �  
                      p                             {  €     0                .file   �  ��  gpesect.c              i  �                           �  �          �  �          �  @          �  �          �  0          �  p                         @          /  �      .text   �     �  	             .data   �                        .bss    �	                       .xdata  p     `                 .pdata  €     x                      ��     r  b                 #       R                    1  	      �                    <  �  
   0                    K       �                    Y  �  
   �                   e  �     T                     p                             {  �     �               .file   �  ��  g    Q            .text   P                       .data   �                        .bss    �	                            q�                        #  T     *                     <    
                        Y  �  
   g                     p  @                      .file   �  ��  gtlsthrd.c             e  P                           �  �	          �  �	          �  �          �  �	          �  @          �  �      .text   P     '  #             .data   �                        .bss    �	     H                 .xdata  �     ,                 .pdata  �     0                      t�     �  3                 #  ~     �                    1  �%     )                    <  0  
   0                    K  �     `                     Y    
   x                   p  `                          {  @	                    .file   �  ��  gtlsmcrt.c         .text   €                       .data   �                       .bss     
                             �     �                    #  3     *                     <  `  
                        Y  y  
   ]                     p  €                      .file   �  ��  gfake                    暴     �                    #  ]                          Y  �  
   {                .text   €     2                 .data                           .bss     
                           <  €  
   0                    {  P
     H                .file   �  ��  glibgcc2.c         .text   �                       .data                           .bss     
                             [�     �                   #  q     s                     <  �  
                        Y  Q  
   �                     p  �                      .file   "  ��  gmingw_matherr.c   .text   �                       .data                          .bss     
                             O�     �                    #  �     *                     <  �  
                        Y  �  
   d                     p  �                      .text   �      .data         .bss     
      .idata$7d      .idata$5�      .idata$4�       .idata$6�      .text   �      .data         .bss     
      .idata$7|      .idata$5�      .idata$4,      .idata$6�      .text   �      .data         .bss     
      .idata$7l      .idata$5�      .idata$4      .idata$6�      .file   �  ��  g    �	                	  �                       handler  
          )	  �          H	  �          l	  �      .text   �                     .data                        .bss     
                      .xdata  �                      .pdata  (                           �     `  
                 #       .                    <  �  
   0                    Y  S  
   b                   p  �                          {  �
     H                .text          .data          .bss    
      .idata$7�      .idata$5d      .idata$4�      .idata$6�      .text         .data          .bss    
      .idata$7�      .idata$5�      .idata$4�      .idata$6�      .text         .data          .bss    
      .idata$7�      .idata$5l      .idata$4�      .idata$6�      .text         .data          .bss    
      .idata$7�      .idata$5      .idata$4L      .idata$62      .text          .data          .bss    
      .idata$7�      .idata$5�      .idata$4D      .idata$6$      .text   (      .data          .bss    
      .idata$7�      .idata$5      .idata$4\      .idata$6F      .text   0      .data          .bss    
      .idata$7�      .idata$5D      .idata$4�      .idata$6€      .text   8      .data          .bss    
      .idata$7p      .idata$5�      .idata$4      .idata$6�      .text   8      .data          .bss    
      .idata$7�      .idata$5�      .idata$4<      .idata$6      .text   8      .data          .bss    
      .idata$7�      .idata$5      .idata$4T      .idata$6<      .text   8      .data          .bss    
      .idata$7�      .idata$5|      .idata$4�      .idata$6�      .text   @      .data          .bss    
      .idata$7�      .idata$5t      .idata$4�      .idata$6�      .text   H      .data          .bss    
      .idata$7€      .idata$5�      .idata$44      .idata$6      .text   P      .data          .bss    
      .idata$7t      .idata$5�      .idata$4      .idata$6�      .text   X      .data          .bss    
      .idata$7�      .idata$5L      .idata$4�      .idata$6�      .text   `      .data          .bss    
      .idata$7�      .idata$5\      .idata$4�      .idata$6�      .text   h      .data          .bss    
      .idata$7�      .idata$5�      .idata$4�      .idata$6�      .text   p      .data          .bss    
      .idata$7�      .idata$54      .idata$4|      .idata$6n      .text   x      .data          .bss    
      .idata$7�      .idata$5�      .idata$4�      .idata$6�      .text   €      .data          .bss    
      .idata$7�      .idata$5      .idata$4d      .idata$6R      .text   �      .data          .bss    
      .idata$7h      .idata$5�      .idata$4      .idata$6�      .text   �      .data          .bss    
      .idata$7�      .idata$5,      .idata$4t      .idata$6d      .text   �      .data          .bss    
      .idata$7�      .idata$5$      .idata$4l      .idata$6Z      .text   �      .data          .bss    
      .idata$7x      .idata$5�      .idata$4$      .idata$6�      .text   �      .data          .bss    
      .idata$7�      .idata$5�      .idata$4�      .idata$6�      .text   �      .data          .bss    
      .idata$7�      .idata$5<      .idata$4�      .idata$6v      .text   �      .data          .bss    
      .idata$7�      .idata$5T      .idata$4�      .idata$6�      .file     ��  gfake              hname   �       fthunk  �      .text   �                       .data                           .bss    
                       .idata$2                      .idata$4�       .idata$5�      .file   �  ��  gfake              .text   �                       .data                           .bss    
                       .idata$4�                      .idata$5�                      .idata$7�                      .text   �      .data          .bss    
      .idata$7<      .idata$5|      .idata$4�       .idata$6,      .text   �      .data          .bss    
      .idata$78      .idata$5t      .idata$4�       .idata$6      .text   �      .data          .bss    
      .idata$7      .idata$5$      .idata$4l       .idata$6,      .text   �      .data          .bss    
      .idata$7P      .idata$5�      .idata$4�       .idata$6�      .text   �      .data          .bss    
      .idata$7L      .idata$5�      .idata$4�       .idata$6r      .text   �      .data          .bss    
      .idata$7      .idata$5      .idata$4d       .idata$6      .text   �      .data          .bss    
      .idata$7(      .idata$5T      .idata$4�       .idata$6�      .text   �      .data          .bss    
      .idata$7      .idata$5,      .idata$4t       .idata$6>      .text          .data          .bss    
      .idata$7      .idata$5      .idata$4T       .idata$6�      .text         .data          .bss    
      .idata$7      .idata$5      .idata$4\       .idata$6      .text         .data          .bss    
      .idata$7      .idata$54      .idata$4|       .idata$6X      .text         .data          .bss    
      .idata$7$      .idata$5L      .idata$4�       .idata$6�      .text          .data          .bss    
      .idata$7,      .idata$5\      .idata$4�       .idata$6�      .text   (      .data          .bss    
      .idata$70      .idata$5d      .idata$4�       .idata$6�      .text   0      .data          .bss    
      .idata$74      .idata$5l      .idata$4�       .idata$6�      .text   8      .data          .bss    
      .idata$7H      .idata$5�      .idata$4�       .idata$6V      .text   @      .data          .bss    
      .idata$7       .idata$5      .idata$4L       .idata$6�      .text   H      .data          .bss    
      .idata$7@      .idata$5�      .idata$4�       .idata$64      .text   P      .data          .bss    
      .idata$7�      .idata$5�      .idata$4D       .idata$6�      .text   X      .data          .bss    
      .idata$7D      .idata$5�      .idata$4�       .idata$6H      .text   `      .data          .bss    
      .idata$7       .idata$5D      .idata$4�       .idata$6�      .text   h      .data          .bss    
      .idata$7�      .idata$5�      .idata$4<       .idata$6�      .text   p      .data          .bss    
      .idata$7      .idata$5<      .idata$4�       .idata$6h      .file   �  ��  gfake              hname   <       fthunk  �      .text   €                       .data                           .bss    
                       .idata$2                       .idata$4<       .idata$5�      .file   �  ��  gfake              .text   €                       .data                           .bss    
                       .idata$4�                       .idata$5�                      .idata$7T     
                 .file   �  ��  gcrtend.c          .text   €                       .data                           .bss    
                           p                         __xc_z         putchar 8          �	   H@ ��       �	  $          �	  4          �	  T          
              
  �          "
            /
        printf  @          ;
  l          R
  �          n
  <      _lock   €          {
      	        �
  �          �
    @ ��       �
  €      __xl_a  8           �
  �          �
  �          �
  @	                   _cexit              �          -  �          L      ��       d     ��       }  P          �  �          �      ��       �     ��       �  8           �  �      __xl_d  H           �        _tls_end`   	                     5  �          B             T  p          i  �          z  8           �  P          �      	    memcpy            �  
          �  P          �             �  €           
            0
  �      malloc             O
         _CRT_MT �           c
  X          o
  H          €
              �
  �          �
  |          �
             �
  (          �
     ��       	                         0  T          ;  �          O  d          l  �          {  0           �  X           �  D          �  (           �  �          �  €          �  4            �      abort   p          (            R  X       __dll__     ��       b      ��       w  ,          �  P          �             �             �    @ ��       �  �          �                  ��         �       calloc  �          5      	        ?  8          X  €	          j  �          v  l          �   H@ ��   fprintf X          �  T      Sleep   �          �  `          �             �  @          �  \          �  €            �                        1  `
      __xi_z  0           =  �          ]            j             �  �           �             �  �          �  �          �  �          �  L	      signal  x             
                        +  0
          9        strncmp �          S  �          f  �          }  €          �  0          �  �          �      ��       �  L          �  �          �  d                                      *  @          I     ��       \  <          |  �          �  �          �  p          �  D          �     ��       �  �          �  �          �  �            �                      !  �      __xl_z  P       __end__    	        @  t          b  p          y  $          �  �          �  0      __xi_a             �  `          �  �          �  |          �  `      __xc_a              �  �               ��         X           .     ��   _fmode  p	          <             G  0          d  �          v  @          �  H          �  L          �  �      __xl_c  @           �  h   	        �  8
          �            �  �                         D	          '  P	          3  �          D  �          \  @      _newmode`	          s  (      fwrite  `          }  �          �  0          �      ��       �      ��       �  €          �  (
          �  
             �                  _onexit �          #         exit    0          5            Z     ��       v      ��       �  ,          �  �          �  H	      _charmax�           �  t          �  �      strlen            �            �  �            h          /  \      stus    @           G             i  �      _unlock �          u            �  �          �  X           �  �      vfprintfh      free    �          �         �  .debug_aranges .debug_info .debug_abbrev .debug_line .debug_frame .debug_str .debug_loc .debug_ranges __mingw_invalidParameterHandler pre_c_init .rdata$.refptr.mingw_initltsdrot_force .rdata$.refptr.mingw_initltsdyn_force .rdata$.refptr.mingw_initltssuo_force .rdata$.refptr.mingw_initcharmax .rdata$.refptr.__image_base__ .rdata$.refptr.mingw_app_type managedapp .rdata$.refptr.__onexitend .rdata$.refptr.__onexitbegin .rdata$.refptr._fmode .rdata$.refptr.__imp__fmode .rdata$.refptr._MINGW_INSTALL_DEBUG_MATHERR pre_cpp_init .rdata$.refptr._newmode startinfo .rdata$.refptr._dowildcard __tmainCRTStartup .rdata$.refptr.__native_startup_lock .rdata$.refptr.__native_startup_state has_cctor .rdata$.refptr.__dyn_tls_init_callback .rdata$.refptr.__mingw_oldexcpt_handler .rdata$.refptr.__imp__acmdln .rdata$.refptr.__imp___initenv .rdata$.refptr.__xc_z .rdata$.refptr.__xc_a .rdata$.refptr.__xi_z .rdata$.refptr.__xi_a WinMainCRTStartup .l_startw mainCRTStartup .rdata$.refptr._gnu_exception_handler .rdata$.refptr._matherr .CRT$XCAA .CRT$XIAA .debug_info .debug_abbrev .debug_loc .debug_aranges .debug_ranges .debug_line .debug_str .rdata$zzz .debug_frame _decode_pointer _encode_pointer __mingw_raise_matherr stUserMathErr __mingw_setusermatherr __report_error __write_memory.part.0 maxSections _pei386_runtime_relocator was_init.73980 .rdata$.refptr.__RUNTIME_PSEUDO_RELOC_LIST_END__ .rdata$.refptr.__RUNTIME_PSEUDO_RELOC_LIST__ __mingw_SEH_error_handler __mingw_init_ehandler was_here.73812 emu_pdata emu_xdata _gnu_exception_handler __do_global_dtors __do_global_ctors .rdata$.refptr.__CTOR_LIST__ initialized __security_init_cookie .data$__security_cookie .data$__security_cookie_complement __report_gsfailure GS_ContextRecord GS_ExceptionRecord GS_ExceptionPointers __dyn_tls_dtor __dyn_tls_init .rdata$.refptr._CRT_MT __tlregdtor mingw_onexit .rdata$.refptr.__imp__onexit my_lconv_init _ValidateImageBase.part.0 _ValidateImageBase _FindPESection _FindPESectionByName __mingw_GetSectionForAddress __mingw_GetSectionCount _FindPESectionExec _GetPEImageBase _IsNonwritableInCurrentImage __mingw_enum_import_library_names pseudo-reloc-list.c __mingwthr_run_key_dtors.part.0 __mingwthr_cs key_dtor_list ___w64_mingwthr_add_key_dtor __mingwthr_cs_init ___w64_mingwthr_remove_key_dtor __mingw_TLScallback mingw_get_invalid_parameter_handler _get_invalid_parameter_handler mingw_set_invalid_parameter_handler _set_invalid_parameter_handler invalid_parameter_handler.c ___RUNTIME_PSEUDO_RELOC_LIST__ __imp_GetStartupInfoA __imp_abort __lib64_libkernel32_a_iname __data_start__ ___DTOR_LIST__ __imp__fmode __imp__lock __imp_RtlVirtualUnwind SetUnhandledExceptionFilter __imp_calloc ___tls_start__ .refptr.__native_startup_state __ImageBase .refptr.__imp__onexit GetLastError GetSystemTimeAsFileTime mingw_initltssuo_force __rt_psrelocs_start __imp___dllonexit .refptr.mingw_initltsdyn_force __dll_characteristics__ __size_of_stack_commit__ __iob_func __imp__acmdln __size_of_stack_reserve__ __major_subsystem_version__ ___crt_xl_start__ __imp_DeleteCriticalSection __imp__set_invalid_parameter_handler .refptr.__CTOR_LIST__ VirtualQuery ___crt_xi_start__ .refptr.__imp__fmode __imp__amsg_exit ___crt_xi_end__ .refptr.__imp___initenv _tls_start __mingw_winmain_lpCmdLine .refptr._matherr .refptr.__RUNTIME_PSEUDO_RELOC_LIST__ __mingw_oldexcpt_handler __imp_GetCurrentThreadId .refptr.mingw_initltssuo_force GetCurrentProcessId TlsGetValue TerminateProcess __bss_start__ __imp___C_specific_handler __imp_putchar ___RUNTIME_PSEUDO_RELOC_LIST_END__ RtlLookupFunctionEntry __size_of_heap_commit__ __imp_GetLastError .refptr._dowildcard __imp_free .refptr.__onexitend __imp_RtlLookupFunctionEntry VirtualProtect mingw_app_type ___crt_xp_start__ __imp_LeaveCriticalSection __mingw_pinit __C_specific_handler .refptr.mingw_initcharmax __imp_GetTickCount .refptr.__mingw_oldexcpt_handler .refptr.__RUNTIME_PSEUDO_RELOC_LIST_END__ ___crt_xp_end__ __minor_os_version__ __imp_GetSystemTimeAsFileTime EnterCriticalSection _MINGW_INSTALL_DEBUG_MATHERR .refptr.__xi_a __image_base__ .refptr._CRT_MT RtlCaptureContext __section_alignment__ __native_dllmain_reason _tls_used UnhandledExceptionFilter mingw_initcharmax __IAT_end__ __imp_memcpy __RUNTIME_PSEUDO_RELOC_LIST__ __imp_RtlAddFunctionTable .refptr._newmode mingw_pcppinit __data_end__ __imp_fwrite __CTOR_LIST__ __imp___getmainargs _head_lib64_libkernel32_a __bss_end__ .refptr.mingw_initltsdrot_force GetTickCount _head_lib64_libmsvcrt_a __native_vcclrit_reason ___crt_xc_end__ RtlAddFunctionTable .refptr.__native_startup_lock __imp_EnterCriticalSection _tls_index __native_startup_state ___crt_xc_start__ __onexitbegin __imp_GetCurrentProcessId __imp___lconv_init __imp_TerminateProcess ___CTOR_LIST__ .refptr.__dyn_tls_init_callback __imp_signal __rt_psrelocs_size __imp_QueryPerformanceCounter __imp_strlen __imp_malloc __mingw_winmain_nShowCmd mingw_pcinit .refptr._gnu_exception_handler __file_alignment__ __imp_InitializeCriticalSection __lconv_init __getmainargs InitializeCriticalSection __imp_exit __major_os_version__ __imp_vfprintf __imp___initenv .refptr.__onexitbegin __IAT_start__ __imp__cexit __imp_UnhandledExceptionFilter __imp_SetUnhandledExceptionFilter .refptr.mingw_app_type __imp__onexit __DTOR_LIST__ RtlVirtualUnwind .refptr.__imp__acmdln __set_app_type __imp_Sleep LeaveCriticalSection __imp___setusermatherr __size_of_heap_reserve__ ___crt_xt_start__ __subsystem__ _amsg_exit __security_cookie_complement __imp_TlsGetValue GetCurrentProcess __setusermatherr __imp_fprintf __imp_VirtualProtect ___tls_end__ __onexitend QueryPerformanceCounter __imp_VirtualQuery __imp__initterm mingw_initltsdyn_force _dowildcard __imp___iob_func __dyn_tls_init_callback .refptr.__image_base__ _initterm __imp_strncmp .refptr._fmode __major_image_version__ __loader_flags__ ___chkstk_ms __native_startup_lock __mingw_winmain_hInstance GetStartupInfoA GetCurrentThreadId __rt_psrelocs_end __imp__get_invalid_parameter_handler __minor_subsystem_version__ __minor_image_version__ __imp__unlock __imp___set_app_type mingw_initltsdrot_force __imp_printf .refptr.__xc_a .refptr.__xi_z .refptr._MINGW_INSTALL_DEBUG_MATHERR DeleteCriticalSection __imp_RtlCaptureContext __RUNTIME_PSEUDO_RELOC_LIST_END__ __dllonexit __imp_GetCurrentProcess .refptr.__xc_z ___crt_xt_end__ __lib64_libmsvcrt_a_iname __security_cookie 