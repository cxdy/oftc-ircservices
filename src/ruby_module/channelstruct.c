#include <ruby.h>
#include "libruby_module.h"

static VALUE ChannelStruct_Initialize(VALUE, VALUE);
static VALUE ChannelStruct_Name(VALUE);
static VALUE ChannelStruct_NameSet(VALUE, VALUE);
static VALUE ChannelStruct_Topic(VALUE);
static VALUE ChannelStruct_TopicSet(VALUE, VALUE);
static VALUE ChannelStruct_TopicInfo(VALUE);
static VALUE ChannelStruct_TopicInfoSet(VALUE, VALUE);
static VALUE ChannelStruct_Mode(VALUE);
static VALUE ChannelStruct_ModeSet(VALUE, VALUE);
static VALUE ChannelStruct_RegChan(VALUE);
static VALUE ChannelStruct_RegChanSet(VALUE, VALUE);

static VALUE
ChannelStruct_Initialize(VALUE self, VALUE channel)
{
  rb_iv_set(self, "@realptr", channel);
  return self;
}

static VALUE
ChannelStruct_Name(VALUE self)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  return rb_str_new2(channel->chname);
}

static VALUE
ChannelStruct_NameSet(VALUE self, VALUE name)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  /* TODO check against CHANNELLEN and throw exception */
  /*channel->chname = StringValueCStr(name);*/
  return name;
}

static VALUE
ChannelStruct_Topic(VALUE self)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  return rb_str_new2(channel->topic);
}

static VALUE
ChannelStruct_TopicSet(VALUE self, VALUE value)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  channel->topic = StringValueCStr(value);
  return value;
}

static VALUE
ChannelStruct_TopicInfo(VALUE self)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  return rb_str_new2(channel->topic_info);
}

static VALUE
ChannelStruct_TopicInfoSet(VALUE self, VALUE value)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  channel->topic_info = StringValueCStr(value);
  return value;
}

static VALUE
ChannelStruct_Mode(VALUE self)
{
  /* TODO Channel Mode */
  return Qnil;
}

static VALUE
ChannelStruct_ModeSet(VALUE self, VALUE value)
{
  /* TODO Channel Mode */
  return Qnil;
}

static VALUE
ChannelStruct_RegChan(VALUE self)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  if(channel->regchan)
  {
    VALUE regchan = rb_cregchan2rbregchan(channel->regchan);
    return regchan;
  }
  else
  {
    return Qnil;
  }
}

static VALUE
ChannelStruct_RegChanSet(VALUE self, VALUE value)
{
  struct Channel *channel = rb_rbchannel2cchannel(self);
  channel->regchan = rb_rbregchan2cregchan(value);
  return value;
}

void
Init_ChannelStruct(void)
{
  cChannelStruct = rb_define_class("ChannelStruct", rb_cObject);

  rb_define_class_variable(cChannelStruct, "@@realptr", Qnil);

  rb_define_method(cChannelStruct, "initialize", ChannelStruct_Initialize, 1);
  rb_define_method(cChannelStruct, "name", ChannelStruct_Name, 0);
  rb_define_method(cChannelStruct, "name=", ChannelStruct_NameSet, 1);
  rb_define_method(cChannelStruct, "topic", ChannelStruct_Topic, 0);
  rb_define_method(cChannelStruct, "topic=", ChannelStruct_TopicSet, 1);
  rb_define_method(cChannelStruct, "topic_info", ChannelStruct_TopicInfo, 0);
  rb_define_method(cChannelStruct, "topic_info=", ChannelStruct_TopicInfoSet, 1);
  rb_define_method(cChannelStruct, "mode", ChannelStruct_Mode, 0);
  rb_define_method(cChannelStruct, "mode=", ChannelStruct_ModeSet, 1);
  rb_define_method(cChannelStruct, "regchan", ChannelStruct_RegChan, 0);
  rb_define_method(cChannelStruct, "regchan=", ChannelStruct_RegChanSet, 1);

  /*TODO members, invites, bans, excepts, invex */
}

struct Channel*
rb_rbchannel2cchannel(VALUE self)
{
  struct Channel* out;
  VALUE channel = rb_iv_get(self, "@realptr");
  Data_Get_Struct(channel, struct Channel, out);
  return out;
}

VALUE
rb_cchannel2rbchannel(struct Channel *channel)
{
  VALUE fc2params, rbchannel, real_channel;
  int status;

  rbchannel = Data_Wrap_Struct(rb_cObject, 0, 0, channel);

  fc2params = rb_ary_new();
  rb_ary_push(fc2params, cChannelStruct);
  rb_ary_push(fc2params, rb_intern("new"));
  rb_ary_push(fc2params, 1);
  rb_ary_push(fc2params, (VALUE)&rbchannel);

  real_channel = rb_protect(RB_CALLBACK(rb_singleton_call), fc2params, &status);

  if(ruby_handle_error(status))
  {
    ilog(L_DEBUG, "RUBY ERROR: Ruby Failed To Create ChannelStruct");
    return Qnil;
  }

  return real_channel;
}

