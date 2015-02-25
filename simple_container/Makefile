# @file makefile
# @brief 多功能通用Makefile。可以生成动态库、静态库、可执行文件，导出头文件
# @author LJA
# @version 0.0.2
# @date 2012-11-11
# @changetime: 2014-05-07

##########################################################
#                    必须设置的变量                      
#
# 这些变量不能为空，否则Make时无法正常运行
#
##########################################################
#编译动作，
#  ar: 编译静态库  so: 编译动态库 lib: 动态库和静态库 app: 编译可执行文件
#  export: 只导出目标文件,不需要编译过程
#  unexport: 将导出到目标目录的文件，从目标目录中删除
#  只能指定一个动作，ar so lib app export只能选择一个
#
#
#  如果不指定动作，可以直接执行 
#  make static; make dynamic; make lib; make app; make export; make unexport
ACTION = app

#目标名称，如果设置为XXX，那么:
# 得到的静态库为: libXXX.a
# 得到的动态库为: libXXX.so
# 得到的可执行文件为: XXX
TARGET = liker

#编译器 
CC = gcc 

#连接器
LD = ld

#归档,静态库
AR = ar

#源代码文件后缀，可以指定多个后缀
#    注意: 目前不会根据后缀的不同而采用不同的编译
#    SUFFIX将被排序去重
#    指编译当前目录下.SUFFIX文件
#    SUFFIX的值不能够使d和o，因为中间生成的会使用这两个后缀
# 例:c
SUFFIX = c
#源码子目录，只能是当前路径下目录
#子目录下的源码的中间文件生在在OBJ_DIR目录下的同名目录中
#OBJ_DIR目录下的同名目录将会自动创建
SUB_SRC_DIR = 

##########################################################
#                    可选的变量
#
# 这些变量根据实际的需求进行设置，如果为空，使用默认值
#
##########################################################
#最终目标被复制到指定放置的路径，可以指定多个路径
#当前目录下会一直保存一份目标文件
#例:TARGET_DIR1 TARGET_DIR2
TARGET_DIR = 

#文件导出
#将$(EXPORT_FILE)中指定的文件复制到$(EXPORT_FILE)目录中
#    如果$(EXPORT_FILE)不为空，$(EXPORT_FILE_DIR)为空，提示error
#    如果$(EXPORT_FILE)为空，$(EXPORT_FILE_DIR)为空，不做操作
#    如果$(EXPORT HEAD_DIR)中有多个目录，文件被复制到其中的每一个目录下
#    EXPORT_FILE和EXPORT_FILE_DIR将被排序去重
#    如果文件路径中包含目录，会在导出位置创建相应的目录
EXPORT_FILE = 
EXPORT_FILE_DIR = 

#编译选项
CFLAGS += -g -Wall

#在$(CC)中使用的连接选项
#    当前的链接过程使用的是$(CC),由$(CC)自行调用链接器进行链接
#    一般情况下，这样已经足够了
#    以后如果有需要，改成直接使用$(LD)进行链接的方式
LDFLAGS =

#中间文件存放路径，只能指定一个路径，默认为当前路径
#例:obj
#Now it is defined in Makefile.pub
OBJ_DIR = ./tmp

#头文件路径，可以指定多个路径，当前路径始终包含在内
#例: ../include
HEAD_DIR += 

#连接库路径
#例: ../lib 
LIB_DIR +=
#动态连接库,
#例m
DY_LIB +=  uuid
#静态连接库,
#例:cunit
ST_LIB += 

#进入其中执行Make的子目录
#    如果指定了子目录，首先按照指定的顺序到子目录下执行make
#    可以指定多个目录，如果目录重复出现，那么每遇到一次就会进入执行一次
#    为了防止无限的循环Make，这里只允许当前路径下目录,只允许如下样式的目录
#        aaa 或者 aaa/
#
#    当前目录中目标的编译在子目录中的Make完成后进行
#    当前Makefile中的变量不会传递到子Makefile中
#
#    虽然需要在每个子目录中设置Makefilek看起来似乎有些麻烦
#    但我认为这样是有必要的，这样我们可以掌控每个Make的行为
SUB_MAKE_DIR = 

# 在当前的Make结束之后，进入下面的目录进行make
# 其他要求与SUB_MAKE_DIR相同
# 不同的是SUB_MAKE_DIR是在当前目录的make之前进行make
# LAST_SUB_MAKE_DIR是在当前目录的make结束后进行make
LAST_SUB_MAKE_DIR = 

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#     实现部分！
#
#     除非知道正在做什么，否则不要修改接下来的任意一个字符!
#
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

##########################################################
#                   自定义函数
#
# 通过shell完成 
#
##########################################################
#检查目录是否存在，如果存在返回空，否则返回不为空
define check_dir
$(shell if [ -d $1 ];then echo;else echo 1;fi)
endef

#检查$1中的值$2所指向的目录是否存在，如果存在返回，否则提示然后立即退出
# 例:
# $(call check_dir2,DIR,xxx) 
# 检查xxx(xxx是变量DIR值中的一个词)目录是否存在，如果不存在，提示:
#   DIR's value (xxx) does not exist!
define check_dir2
$(if $(call check_dir,$2),$(error "$1's value ($2) does not exist!"),)
endef

#检查文件是否存在，如果存在返回空，否则返回不为空
define check_file
$(shell if [ -e $1 ];then echo;else echo 1;fi)
endef

#检查$1中的值$2所指向的文件是否存在，如果存在返回，否则提示然后立即退出
# 例:
# $(call check_file2,FILE,xxx) 
# 检查xxx(xxx是变量FILE值中的一个词)文件是否存在，如果不存在，提示:
#   FILE's value (xxx) does not exist!
define check_file2
$(if $(call check_file,$2),$(error "$1's value ($2) does not exist!"),)
endef

#生成一个变量obj-XX,其中存放所有以XX为后缀的文件
#  在DIR目录下对应的.o文件
#例:  $(call set_obj_x,c,DIR,$(all_src))
#  定义了obj-c=DIR/XXX.o
define set_obj_x
obj-$1 = $(patsubst %.$1,$2/%.o,$3)
endef

##########################################################
#                   变量检查
#
# 检查设置的变量是否正确
#
##########################################################

## Check ACTION
ifeq ($(MAKECMDGOALS),)
ACTION := $(strip $(ACTION))
ifeq ($(ACTION),)
$(error "ACTION is not set!")
endif
ifneq ($(words $(ACTION)),1)
$(error "ACTION's value is too many!")
endif
allact = ar so lib app export
ifeq ($(allact),$(filter-out $(ACTION),$(allact)))
$(error "ACTION's value ($(ACTION)) is invalid!")
endif
endif

## Check CC
ifneq ($(ACTION), export)
CC := $(strip $(CC))
ifeq ($(CC),)
$(error "CC is not set!")
endif
endif

## Check LD
ifneq ($(ACTION), export)
LD := $(strip $(LD))
ifeq ($(LD),)
$(error "LD is not set!")
endif
endif

## Check AR, when ACTION's value is ar
ifeq ($(ACTION),ar)
AR := $(strip $(AR))
ifeq ($(AR),)
$(error "AR is not set!")
endif
endif

## Check SUFFIX

ifneq ($(ACTION), export)
SUFFIX := $(strip $(SUFFIX))
SUFFIX := $(sort $(SUFFIX))
ifeq ($(SUFFIX),)
$(error "SUFFIX is not set!")
endif
ifeq ($(SUFFIX),d)
$(error "SUFFIX's value (d) is invalid!")
endif
ifeq ($(SUFFIX),o)
$(error "SUFFIX's value (o) is invalid!")
endif
endif

## Check SUB_SRC_DIR
SUB_SRC_DIR := $(strip $(SUB_SRC_DIR))
ifneq ($(SUB_SRC_DIR),)
$(foreach var,$(SUB_SRC_DIR),$(call check_dir2,SUB_SRC_DIR,$(var)))
endif

## Check TARGET
ifneq ($(ACTION), export)
TARGET := $(strip $(TARGET))
ifeq ($(TARGET),)
$(error "TARGET is not set!")
endif
endif

## Check CFLAGS
CFLAGS := $(strip $(CFLAGS))

## Check LDFLAGS
LDFLAGS := $(strip $(LDFLAGS))

## Check OBJ_DIR
OBJ_DIR := $(strip $(OBJ_DIR))
ifeq ($(OBJ_DIR),)
OBJ_DIR=./
endif
ifneq ($(words $(sort $(OBJ_DIR))),1)
$(error "OBJ_DIR's value is too many!")
endif
ifneq ($(call check_dir,$(OBJ_DIR)),)
$(error "OBJ's value ($(OBJ_DIR)) does not exist!")
endif

## Check HEAD_DIR
ifneq ($(ACTION), export)
HEAD_DIR := $(strip $(HEAD_DIR))
ifneq ($(HEAD_DIR),)
$(foreach var,$(HEAD_DIR),$(call check_dir2,HEAD_DIR,$(var)))
endif
endif

## Check LIB_DIR
ifneq ($(ACTION), export)
LIB_DIR := $(strip $(LIB_DIR))
ifneq ($(LIB_DIR),)
$(foreach var,$(LIB_DIR),$(call check_dir2,LIB_DIR,$(var)))
endif
endif

## Check DY_LIB
DY_LIB := $(strip $(DY_LIB))

## Check ST_LIB
ST_LIB := $(strip $(ST_LIB))

## Check TARGET_DIR
ifneq ($(ACTION), export)
TARGET_DIR := $(strip $(TARGET_DIR))
ifneq ($(TARGET_DIR),)
$(foreach var,$(TARGET_DIR),$(call check_dir2,TARGET_DIR,$(var)))
endif
endif

## Check EXPORT_FILE
EXPORT_FILE := $(strip $(EXPORT_FILE))
EXPORT_FILE := $(sort $(EXPORT_FILE))
EXPORT_FILE_DIR := $(strip $(EXPORT_FILE_DIR))
EXPORT_FILE_DIR := $(sort $(EXPORT_FILE_DIR))
ifneq ($(EXPORT_FILE),)
$(foreach var,$(EXPORT_FILE_DIR),$(call check_dir2,EXPORT_FILE_DIR,$(var)))
$(foreach var,$(EXPORT_FILE),$(call check_file2,EXPORT_FILE,$(var)))
endif

## Check SUB_MAKE_DIR
SUB_MAKE_DIR := $(strip $(SUB_MAKE_DIR))
$(foreach var,$(SUB_MAKE_DIR),$(call check_dir2,SUB_MAKE_DIR,$(var)))

## Check LAST_SUB_MAKE_DIR
LAST_SUB_MAKE_DIR := $(strip $(LAST_SUB_MAKE_DIR))
$(foreach var,$(LAST_SUM_MAKE_DIR),$(call check_dir2,LAST_SUB_MAKE_DIR,$(var)))

##########################################################
#
#                   根据配置生成需要的变量
#
##########################################################

#所有.SUFFIX文件
all_src = $(foreach i,$(SUFFIX),$(wildcard ./*.$(i) $(patsubst %,./%/*.$(i),$(SUB_SRC_DIR))))

#定义了obj-X变量,x是SUFFIX中的值
$(eval $(foreach i,$(SUFFIX),$(call set_obj_x,$i,$(OBJ_DIR),$(all_src))))

#所有.SUFFIX文件对应的.o文件
all_obj = $(foreach i,$(SUFFIX),$(obj-$i))

#头文件路径
head_dir = $(foreach d,$(HEAD_DIR),-I$d)

#连接库路径
lib_dir = $(foreach d,$(LIB_DIR),-L$d)

#动态连接库
dy_lib	= $(foreach d,$(DY_LIB),-l$d)

#静态连接库
st_lib	= $(foreach d,$(ST_LIB),-l$d)

#最终的编译选项
cflags = $(head_dir) $(CFLAGS) 

#最终的连接选项
ldflags = $(lib_dir) -Wl,-Bstatic $(st_lib) -Wl,-Bdynamic $(dy_lib) $(LDFLAGS)

#静态库目标
static_target = lib$(TARGET).a

#动态库目标
dynamic_target = lib$(TARGET).so

##########################################################
#                   开始编译
#
# 将所有.SUFFIX的文件先获取对应的依赖关系.d文件
# 然后编译成.o文件
# 最后将所有的.o文件连接成目标文TARGET
#
##########################################################
ifeq ($(MAKECMDGOALS),dynamic)  #动态库
all = $(dynamic_target)
cflags += -fpic -shared
endif

ifeq ($(MAKECMDGOALS),static)   #静态库
all = $(static_target)
endif

ifeq ($(ACTION),ar)    #静态库
all = $(static_target)
endif

ifeq ($(ACTION),so)    #动态库
all = $(dynamic_target)
cflags += -fpic -shared
endif

ifeq ($(ACTION),lib)   #动态库和静态库
all = lib
endif

ifeq ($(ACTION),app)   #可执行程序
all = $(TARGET)
endif

ifeq ($(ACTION), export)  #只导出文件
all =
endif

.PHONY = export unexport lib clean 
export: $(all) $(EXPORT_FILE) 
	for i in $(EXPORT_FILE); do for d in $(EXPORT_FILE_DIR);do path=$$d/`dirname $$i`;mkdir -p $$path;cp -rf $$i $$path;echo "$$i --> $$path";done;done
	for d in $(LAST_SUB_MAKE_DIR);do make -C $$d;done;

unexport:
	for i in $(EXPORT_FILE); do for d in $(EXPORT_FILE_DIR);do path=$$d/`dirname $$i`;rm -rf $$path/$$i;done;done
	for i in $(TARGET_DIR);do rm -rf $$i/$(static_target);done
	for i in $(TARGET_DIR);do rm -rf $$i/$(dynamic_target);done
	for i in $(TARGET_DIR);do rm -rf $$i/$(TARGET);done
	

#动态库和静态库
lib: 
	make static
	make clean
	make dynamic
#	make clean

#静态库
static : $(static_target)
$(static_target):$(all_obj)
	for d in $(SUB_MAKE_DIR);do make -C $$d; done;
	$(AR) rcvs $@ $^
	for i in $(TARGET_DIR);do if [ -d $$i ];then cp $@ $$i;fi;done

#动态库
dynamic : $(dynamic_target) 
$(dynamic_target): $(all_obj)
	for d in $(SUB_MAKE_DIR);do make -C $$d; done;
	$(CC) -o $@ $^ $(ldflags) -shared
	for i in $(TARGET_DIR);do if [ -d $$i ];then cp $@ $$i;fi;done

#应用程序
app : $(TARGET)
$(TARGET): $(all_obj)
	for d in $(SUB_MAKE_DIR);do make -C $$d; done;
	$(CC) -o $@ $^ $(ldflags) 
	for i in $(TARGET_DIR);do if [ -d $$i ];then cp $@ $$i;fi;done

clean:
	rm -f $(TARGET) $(static_target) $(dynamic_target)
	for d in $(SUB_MAKE_DIR);do make clean -C $$d; done;
	for d in $(LAST_SUB_MAKE_DIR);do make clean -C $$d; done;
	find $(OBJ_DIR) -name "*.o" -exec rm -f {} +
	find $(OBJ_DIR) -name "*.d" -exec rm -f {} +

#obj文件生成规则
#例:
#  cmd_o  文件后缀($1) obj文件存放的路径($2) 编译器($3) 编译选项($4) 
#$(call cmd_o,c,obj,gcc,-g -O1)第一次展开后:
#   $(obj-c): obj%.o:%.c MAKEFILE_LIST
#   	gcc -g -O1 -Wp,-MT,$@ -Wp,-MMD,$@.d -c -o $@ $<
#  这里的目标文件使用了Makefile的静态模式，可以指定多个目标
#  obj%.o:%.c说明了每一个目标文件的依赖关系
#
#  MAKEFIL_LIST是make的内置变量,是make读取的文件的列表
#  这样确保了Makefile被修改后也会进行重新编译
define cmd_o
$$(obj-$1): $2/%.o:%.$1 $(MAKEFILE_LIST)
	mkdir -p `dirname $$@`
	$3 $4 -Wp,-MT,$$@ -Wp,-MMD,$$@.d -c -o $$@ $$<
endef

#给出了obj-(SUFIX)分别对应的编译规则
#当前是对所有类型后缀的文件使用相同的编译规则$(CC) $(cflags)
#以后有需要的时候需要改成对不同后缀的文件使用不同的编译规则
$(eval $(foreach i,$(SUFFIX),$(call cmd_o,$i,$(OBJ_DIR),$(CC),$(cflags))))

#引入gcc自动生成的依赖文件
-include $(patsubst %.o,%o.d,$(all_obj))
