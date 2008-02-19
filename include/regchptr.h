#ifndef INCLUDED_regchptr_h
#define INCLUDED_regchptr_h

struct RegChannel
{
  dlink_node node;

  unsigned int id;
  time_t regtime;
  char channel[CHANNELLEN+1];
  char *description;
  char *entrymsg;
  char *url;
  char *email;
  char *topic;
  char *mlock;
  char priv;
  char restricted;
  char topic_lock;
  char verbose;
  char autolimit;
  char expirebans;
  char floodserv;
  char autoop;
  char autovoice;
  char leaveops;
  unsigned int expirebans_lifetime;
  /* Per Host */
  struct MessageQueue **flood_hash;
  dlink_list flood_list;
  /* Per Channel */
  struct MessageQueue *gqueue;
};

#endif