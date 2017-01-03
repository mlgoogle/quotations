//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry


#include "net/packet_processing.h"
#include <list>
#include <string>
#include "basic/unzip.h"
#include "basic/zip.h"
#include "protocol/data_packet.h"
#include "net/comm_head.h"
#include "logic/logic_comm.h"
//#include "login/login_cookie.h"

#define DUMPPACKBUF     4096 * 10

namespace net {

bool PacketProsess::PacketStream(const PacketHead* packet_head,
        void ** packet_stream, int32* packet_stream_length) {
    bool r = true;
    /* int16 packet_length = packet_head->packet_length;
    int8 is_zip_encrypt = packet_head->is_zip_encrypt;
    int8 type = packet_head->type;
    int16 signature = packet_head->signature;
    int16 operate_code = packet_head->operate_code;
    int16 data_length = packet_head->data_length;
    int32 timestamp = packet_head->timestamp;
    int64 session_id = packet_head->session_id;
    int32 reserved = packet_head->reserved;


    char* body_stream = NULL;
    char* data = NULL;

    switch (operate_code) {
      case CRAWLER_MGR_REG:
      case ANALYTICAL_REG: {
        struct CrawlerMgrReg* vCrawlerMgrReg =
                (struct CrawlerMgrReg*)packet_head;
        BUILDHEAD(CRAWLER_MGR_REG_SIZE);

        out.Write16(vCrawlerMgrReg->level);
        out.WriteData(vCrawlerMgrReg->password, PASSWORD_SIZE - 1);
        out.WriteData(vCrawlerMgrReg->mac, MAC_SIZE - 1);

        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case CRAWLER_MGR_REGSTATE:  {
        struct CrawlerMgrRegState* vCrawlerMgrRegState =
                (struct CrawlerMgrRegState*)packet_head;
        BUILDHEAD(CRAWLER_MGR_REG_STATE_SIZE);

        out.Write32(vCrawlerMgrRegState->id);
        out.WriteData(vCrawlerMgrRegState->token, TOKEN_SIZE - 1);

        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case CRAWLER_REG_FAILED: {
          struct CrawlerFailed* vCrawlerFailed =
                  (struct CrawlerFailed*)packet_head;
          BUILDHEAD(CRAWLER_FAILED_SIZE);
          out.Write32(vCrawlerFailed->erron_code);
          body_stream = const_cast<char*>(out.GetData());
          break;
      }

      case REPLY_HARDINFO:  {
        struct ReplyHardInfo* vReplyHardInfo =
                (struct ReplyHardInfo*)packet_head;
        BUILDHEAD(REPLY_HARDINFO_SIZE);

        out.Write32(vReplyHardInfo->id);
        out.WriteData(vReplyHardInfo->token, TOKEN_SIZE - 1);
        out.WriteData(vReplyHardInfo->cpu, HADRINFO_SIZE - 1);
        out.WriteData(vReplyHardInfo->mem, HADRINFO_SIZE - 1);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case ASSIGNMENT_SINGLE_TASK:  {
        struct AssignmentSingleTask* vAssignmentSingleTask =
                (struct AssignmentSingleTask*)packet_head;
        BUILDHEAD(ASSIGNMENT_SINGLE_TASK_SIZE);

        out.Write32(vAssignmentSingleTask->id);
        out.Write64(vAssignmentSingleTask->task_id);
        out.Write8(vAssignmentSingleTask->depth);
        out.Write8(vAssignmentSingleTask->machine);
        out.Write8(vAssignmentSingleTask->storage);
        out.WriteData(vAssignmentSingleTask->url, URL_SIZE - 1);

        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case ASSIGNMENT_MULTI_TASK:  {
        struct AssignmentMultiTask* vAssignmentMultiTask =
                (struct AssignmentMultiTask*)packet_head;
        int32 len = 0;
        data_length = vAssignmentMultiTask->task_set.size()
                * TASK_UNIT_SIZE + sizeof(int32);

        BUILDHEAD(data_length);
        out.Write32(vAssignmentMultiTask->id);
        std::list<struct TaskUnit*>::iterator it =
                vAssignmentMultiTask->task_set.begin();

        for ( ; it != vAssignmentMultiTask->task_set.end(); it++) {
            out.Write64((*it)->task_id);
            out.Write64((*it)->pid);
            out.Write64((*it)->attr_id);
            out.Write64((*it)->unix_time);
            out.Write8((*it)->max_depth);
            out.Write8((*it)->current_depth);
            out.Write8((*it)->machine);
            out.Write8((*it)->storage);
            out.Write8((*it)->is_login);
            out.Write8((*it)->is_over);
            out.Write8((*it)->is_forge);
            out.Write8((*it)->method);
            out.WriteData((*it)->url, URL_SIZE - 1);
        }

        body_stream = const_cast<char*>(out.GetData());
        break;
      }
      
      case ASSIGNMENT_DETAIL_TASK:  {
        struct AssignmentDetailTask* vAssignmentDetailTask =
                (struct AssignmentDetailTask*)packet_head;
        int32 len = 0;
        data_length = vAssignmentDetailTask->task_set.size()
                * TASK_UNIT_SIZE + sizeof(int32);

        BUILDHEAD(data_length);
        out.Write32(vAssignmentDetailTask->id);
        std::list<struct TaskUnit*>::iterator it =
                vAssignmentDetailTask->task_set.begin();

        for ( ; it != vAssignmentDetailTask->task_set.end(); it++) {
            out.Write64((*it)->task_id);
            out.Write64((*it)->attr_id);
            out.Write64((*it)->unix_time);
            out.Write8((*it)->max_depth);
            out.Write8((*it)->current_depth);
            out.Write8((*it)->machine);
            out.Write8((*it)->storage);
            out.Write8((*it)->is_login);
            out.Write8((*it)->is_over);
            out.Write8((*it)->is_forge);
            out.Write8((*it)->method);
            out.WriteData((*it)->url, URL_SIZE - 1);
        }

        body_stream = const_cast<char*>(out.GetData());
        break;
      }
      
      case REPLY_TASK_STATE:  {
        struct ReplyTaskState* vReplyTaskState =
                (struct ReplyTaskState*)packet_head;
        BUILDHEAD(REPLYTASKTATE_SIZE);
        out.Write32(vReplyTaskState->id);
        out.WriteData(vReplyTaskState->token, TOKEN_SIZE - 1);
        out.Write64(vReplyTaskState->jobid);
        out.Write8(vReplyTaskState->state);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }
      
      case REPLY_DETAIL_STATE:  {
        struct ReplyDetailState* vReplyDetailState =
                (struct ReplyDetailState*)packet_head;
        BUILDHEAD(REPLYDETAILTATE_SIZE);
        out.Write32(vReplyDetailState->id);
        out.WriteData(vReplyDetailState->token, TOKEN_SIZE - 1);
        out.Write64(vReplyDetailState->jobid);
        out.Write8(vReplyDetailState->state);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case GET_CRAWL_CONTENT_NUM:  {
        struct CrawlContentNum* vCrawlContentNum =
                (struct CrawlContentNum*)packet_head;

        BUILDHEAD(CRAWLCONTENTNUM_SIZE);

        out.Write32(vCrawlContentNum->id);
        out.Write64(vCrawlContentNum->jobid);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case REPLY_CRAWL_CONTENT_NUM:  {
        struct ReplyCrawlContentNum* vReplyCrawlContentNum =
                (struct ReplyCrawlContentNum*)packet_head;

        BUILDHEAD(REPLYCRAWLCONTENTNUM_SIZE);
        out.Write32(vReplyCrawlContentNum->id);
        out.WriteData(vReplyCrawlContentNum->token, TOKEN_SIZE - 1);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case GET_FORGEINFO:  {
        struct GetForgeInfo* vGetForgeInfo =
                (struct GetForgeInfo*)(packet_head);
        BUILDHEAD(GETFORGEINFO_SIZE);
        out.Write32(vGetForgeInfo->id);
        out.WriteData(vGetForgeInfo->token, TOKEN_SIZE - 1);
        out.Write64(vGetForgeInfo->task_id);
        out.Write8(vGetForgeInfo->forge_type);
        out.Write8(vGetForgeInfo->num);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case REPLY_FOGEINFO_UA: {
        struct ReplyUAForgeInfo* vReplyUAForgeInfo =
                  (struct ReplyUAForgeInfo*)packet_head;
        int32 len = 0;
        data_length = vReplyUAForgeInfo->forgen_set.size() *
                  (sizeof(int32) + sizeof(int8) + UA_FORGEN_SIZE - 1) +
                  sizeof(int32) + sizeof(int64);

        BUILDHEAD(data_length);
        out.Write32(vReplyUAForgeInfo->id);
        out.Write64(vReplyUAForgeInfo->task_id);
        std::list<struct UAForgeInfo*>::iterator it =
                vReplyUAForgeInfo->forgen_set.begin();

        for ( ; it != vReplyUAForgeInfo->forgen_set.end(); it++) {
            out.Write32((*it)->id);
            out.Write8((*it)->type);
            out.WriteData((*it)->forgen_info, UA_FORGEN_SIZE - 1);
        }

        body_stream = const_cast<char*>(out.GetData());
        break;
      }
      case REPLY_FOGEINFO_IP: {
        struct ReplyIPForgeInfo* vReplyIPForgeInfo =
                (struct ReplyIPForgeInfo*)packet_head;
        int32 len = 0;
        data_length = vReplyIPForgeInfo->forgen_set.size() *
                (sizeof(int32) + sizeof(int8) + IP_FORGEN_SIZE - 1) +
                sizeof(int32) + sizeof(int64);

        BUILDHEAD(data_length);
        out.Write32(vReplyIPForgeInfo->id);
        out.Write64(vReplyIPForgeInfo->task_id);
        std::list<struct IPForgeInfo*>::iterator it =
                vReplyIPForgeInfo->forgen_set.begin();

        for ( ; it != vReplyIPForgeInfo->forgen_set.end(); it++) {
            out.Write32((*it)->id);
            out.Write8((*it)->type);
            out.WriteData((*it)->forgen_info, IP_FORGEN_SIZE - 1);
        }

        body_stream = const_cast<char*>(out.GetData());
        break;
      }

      case ANALYTICAL_INFO: {
          struct AnalysiInfo* vAnalysiInfo =
                  (struct AnalysiInfo*)packet_head;
          int32 len = 0;

          int32 n = vAnalysiInfo->analysi_set.size();
          int32 n_len = ANALYIUNIT_SIZE;
          data_length = vAnalysiInfo->analysi_set.size() * (ANALYIUNIT_SIZE);

          BUILDHEAD(data_length);
          std::list<struct AnalysiUnit*>::iterator it =
                  vAnalysiInfo->analysi_set.begin();

          for (; it != vAnalysiInfo->analysi_set.end(); it++) {
              out.Write64((*it)->ayalysi_id);
              out.Write64((*it)->task_id);
              out.Write32((*it)->attr_id);
              out.Write8((*it)->type);
              out.Write8((*it)->max_depth);
              out.Write8((*it)->cur_depth);
              out.WriteData((*it)->name, NAME_SIZE - 1);
              out.WriteData((*it)->key, KEY_SIZE - 1);
          }
          body_stream = const_cast<char*>(out.GetData());
          break;
      }
      case ANALYTICAL_STATE: {
          struct AnalysiState* vAnalysiState =
                  (struct AnalysiState*)(packet_head);
          BUILDHEAD(ANALYSISTATE_SIZE);
          out.Write64(vAnalysiState->id);
          out.Write8(vAnalysiState->state);
          body_stream = const_cast<char*>(out.GetData());
          break;
      }
      case ANALYTICAL_URL_SET: {
          struct AnalyticalURLSet* vAnalyticalURLSet =
                  (struct AnalyticalURLSet*)(packet_head);

          int32 len = 0;
          data_length = vAnalyticalURLSet->analytical_url_set.size() *
                  (sizeof(int8) * 3 + sizeof(int64) + URL_SIZE - 1);
          BUILDHEAD(data_length);

          std::list<struct AnalyticalURLUnit*>::iterator it =
                  vAnalyticalURLSet->analytical_url_set.begin();

          for (; it != vAnalyticalURLSet->analytical_url_set.end(); it++) {
              out.Write64((*it)->task_id);
              out.Write64((*it)->attr_id);
              out.Write8((*it)->max_depth);
              out.Write8((*it)->current_depth);
              out.Write8((*it)->method);
              out.WriteData((*it)->url, URL_SIZE-1);
          }
          body_stream = const_cast<char*>(out.GetData());
          break;
      }

      case DELIVERED_COOKIE_SET: {
          struct LoginCookieSet* vLoginCookieSet =
              (struct LoginCookieSet*)(packet_head);

          int32 len = 0;
          data_length = sizeof(int32) + sizeof(int64);
          std::list<struct LoginCookieUnit*>::iterator itr =
              vLoginCookieSet->login_cookie_set.begin();

          for( ; itr != vLoginCookieSet->login_cookie_set.end(); ++itr) {
              data_length = data_length + (*itr)->login_cookie_body.size() + 2;
          }

          BUILDHEAD(data_length);
          out.Write32(vLoginCookieSet->manage_id);
          out.Write64(vLoginCookieSet->attr_id);

          for( itr = vLoginCookieSet->login_cookie_set.begin(); itr !=
                  vLoginCookieSet->login_cookie_set.end(); ++itr) {
              out.Write16((*itr)->len);
              int32 body_length = (*itr)->login_cookie_body.size();
              const char* tmp = (*itr)->login_cookie_body.c_str();
              out.WriteData(tmp, body_length);
          }
          body_stream = const_cast<char*>(out.GetData());
          break;
        }


      case HEART_PACKET:
      case GET_HARDINFO:
      case TEMP_CRAWLER_OP: {
        BUILDHEAD(0);
        body_stream = const_cast<char*>(out.GetData());
        break;
      }
      default:
        r = false;
        break;
    }

    if (r) {
        char* packet = NULL;
        int32 packet_body_len = 0;
        if (is_zip_encrypt == ZIP_AND_NOENCRYPT) {
            uint8* zip_data = NULL;
            uint64 zip_len = 0;
            const uint8* unzip =
                const_cast<const uint8*>(reinterpret_cast<uint8*>(body_stream));
            zip_len = CompressionStream(unzip,
                packet_length - sizeof(int16) - sizeof(int8),
                &zip_data);
            free(body_stream);
            body_stream = NULL;
            packet  = reinterpret_cast<char*>(zip_data);
            packet_body_len = zip_len;
        } else {
            packet = body_stream;
            packet_body_len = packet_length - sizeof(int16) - sizeof(int8);
        }

        packet::DataOutPacket out_packet(false,
            packet_body_len + sizeof(int16) + sizeof(int8));
        out_packet.Write16(packet_body_len + sizeof(int16) + sizeof(int8));
        out_packet.Write8(is_zip_encrypt);
        out_packet.WriteData(packet, packet_body_len);
        free(packet);
        packet = NULL;
        *packet_stream =  reinterpret_cast<void*>(
            const_cast<char*>(out_packet.GetData()));
        *packet_stream_length =  packet_body_len + sizeof(int16) + sizeof(int8);
    }*/

  /*
    packet::DataOutPacket out_packet(false,
            body_len + sizeof(int16) + sizeof(int8));
    out_packet.Write16(body_len + sizeof(int16) + sizeof(int8));
    out_packet.Write8(is_zip_encrypt);
    out_packet.WriteData(body_stream, body_len);
    if (body_stream) {delete [] body_stream; body_stream = NULL;}

    *packet_stream =
            reinterpret_cast<void*>(const_cast<char*>(out_packet.GetData()));

    if (packet_head->data_length == 0)
        *packet_stream_length = body_len + sizeof(int16) + sizeof(int8);
    else
        *packet_stream_length = packet_length;
   */

    return r;
}



bool PacketProsess::UnpackStream(const void* packet_stream, int32 len,
            struct PacketHead** packet_head) {
    bool r = true;
    /*packet::DataInPacket in_packet(reinterpret_cast<const char*>(packet_stream),
            len);
    int16 packet_length = in_packet.Read16();
    int8 is_zip_encrypt = in_packet.Read8();
    char* body_stream = NULL;
    int32 body_length = 0;
    int32 temp = 0;

    // 是否解压解码
    if (is_zip_encrypt == ZIP_AND_NOENCRYPT) {
        char* temp_body_stream =
                reinterpret_cast<char*>(const_cast<void*>(packet_stream))
                + sizeof(int16) + sizeof(int8);
        const uint8* zipdata = reinterpret_cast<uint8*>(temp_body_stream);
        uint8* unzipdata = NULL;
        int32 temp_body_len = len - sizeof(int16) - sizeof(int8);
        body_length = DecompressionStream(zipdata,
                temp_body_len,
                &unzipdata);
        body_stream = reinterpret_cast<char*>(unzipdata);
    } else {
        body_stream =
                reinterpret_cast<char*>(const_cast<void*>(packet_stream)) +
                sizeof(int16) + sizeof(int8);
        body_length = len - sizeof(int16) - sizeof(int8);
    }


    BUILDPACKHEAD();

    switch (operate_code) {
      case CRAWLER_MGR_REG:
      case ANALYTICAL_REG: {
        struct CrawlerMgrReg* vCrawlerMgrReg = new struct CrawlerMgrReg;
        *packet_head = (struct PacketHead*)vCrawlerMgrReg;

        FILLHEAD();

        vCrawlerMgrReg->level = in.Read16();
        int temp = 0;
        memcpy(vCrawlerMgrReg->password, in.ReadData(PASSWORD_SIZE - 1, temp),
                PASSWORD_SIZE - 1);
        int32 password_len = (temp - 1) < (PASSWORD_SIZE - 1) ?
                (temp - 1) : (PASSWORD_SIZE - 1);

        vCrawlerMgrReg->password[password_len] = '\0';

        memcpy(vCrawlerMgrReg->mac, in.ReadData(MAC_SIZE - 1, temp),
                MAC_SIZE - 1);
        int32 mac_len = (temp - 1) < (MAC_SIZE - 1) ?
                (temp - 1) : (MAC_SIZE - 1);
        vCrawlerMgrReg->mac[mac_len] = '\0';

       break;
      }

      case CRAWLER_MGR_REGSTATE: {
        struct CrawlerMgrRegState* vCrawlerMgrRegState =
                new struct CrawlerMgrRegState;
        *packet_head = (struct PacketHead*)vCrawlerMgrRegState;

        FILLHEAD();
        vCrawlerMgrRegState->id = in.Read32();
        int temp = 0;
        memcpy(vCrawlerMgrRegState->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                      (temp - 1) : (TOKEN_SIZE - 1);
             vCrawlerMgrRegState->token[token_len] = '\0';
         break;
      }

      case CRAWLER_REG_FAILED: {
          struct CrawlerFailed* vCrawlerFailed =
                  new struct CrawlerFailed;
          *packet_head = (struct PacketHead*)vCrawlerFailed;

          FILLHEAD();
          vCrawlerFailed->erron_code = in.Read32();
          break;
      }

      case REPLY_HARDINFO: {
        struct ReplyHardInfo* vReplyHardInfo = new struct ReplyHardInfo;
        *packet_head = (struct PacketHead*)vReplyHardInfo;

        FILLHEAD();

        vReplyHardInfo->id = in.Read32();
        int temp = 0;
        memcpy(vReplyHardInfo->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                  (temp - 1) : (TOKEN_SIZE - 1);
         vReplyHardInfo->token[token_len] = '\0';


         memcpy(vReplyHardInfo->cpu, in.ReadData(HADRINFO_SIZE - 1, temp),
                 HADRINFO_SIZE - 1);
         int32 cpu_len = (temp - 1) < (HADRINFO_SIZE - 1) ?
                          (temp - 1) : (HADRINFO_SIZE - 1);
         vReplyHardInfo->cpu[cpu_len] = '\0';


         memcpy(vReplyHardInfo->mem, in.ReadData(HADRINFO_SIZE - 1, temp),
                 HADRINFO_SIZE - 1);
         int32 mem_len = (temp - 1) < (HADRINFO_SIZE - 1) ?
                          (temp - 1) : (HADRINFO_SIZE - 1);
         vReplyHardInfo->mem[mem_len] = '\0';
         break;
      }

      case ASSIGNMENT_SINGLE_TASK: {
        struct AssignmentSingleTask* vAssignmentSingleTask =
                new AssignmentSingleTask;
        *packet_head = (struct PacketHead*)vAssignmentSingleTask;

        FILLHEAD();

        vAssignmentSingleTask->id = in.Read32();
        vAssignmentSingleTask->task_id = in.Read64();
        vAssignmentSingleTask->depth = in.Read8();
        vAssignmentSingleTask->machine = in.Read8();
        vAssignmentSingleTask->storage = in.Read8();
        int temp = 0;
        memcpy(vAssignmentSingleTask->url, in.ReadData(URL_SIZE - 1, temp),
                URL_SIZE - 1);
        int32 url_len = (temp - 1) < (URL_SIZE - 1) ?
                          (temp - 1) : (URL_SIZE - 1);
         vAssignmentSingleTask->url[url_len] = '\0';

         break;
      }

      case ASSIGNMENT_MULTI_TASK : {
        struct AssignmentMultiTask* vAssignmentMultiTask =
                new AssignmentMultiTask;
        *packet_head = (struct PacketHead*)vAssignmentMultiTask;

        FILLHEAD();

        vAssignmentMultiTask->id = in.Read32();

        int32 set_size = data_length - sizeof(int32);
        int32 i = 0;
        int32 num = set_size / (TASK_UNIT_SIZE - 1);
        int32 len = 0;
        for (; i< num; i++) {
            struct TaskUnit* unit = new struct TaskUnit;
            int32 temp = 0;
            unit->task_id = in.Read64();
            unit->pid = in.Read64();
            unit->attr_id = in.Read64();
            unit->unix_time = in.Read64();
            unit->max_depth = in.Read8();
            unit->current_depth = in.Read8();
            unit->machine = in.Read8();
            unit->storage = in.Read8();
            unit->is_login = in.Read8();
            unit->is_over = in.Read8();
            unit->is_forge = in.Read8();
            unit->method = in.Read8();
            memcpy(unit->url, in.ReadData(URL_SIZE - 1, temp),
                    URL_SIZE - 1);
            int32 url_len = (temp - 1) < (URL_SIZE - 1) ?
                              (temp - 1) : (URL_SIZE - 1);
            unit->url[url_len] = '\0';

            vAssignmentMultiTask->task_set.push_back(unit);
        }
        break;
      }

      case ASSIGNMENT_DETAIL_TASK : {
        struct AssignmentDetailTask* vAssignmentDetailTask =
                new AssignmentDetailTask;
        *packet_head = (struct PacketHead*)vAssignmentDetailTask;

        FILLHEAD();

        vAssignmentDetailTask->id = in.Read32();

        int32 set_size = data_length - sizeof(int32);
        int32 i = 0;
        int32 num = set_size / (TASK_UNIT_SIZE - 1);
        int32 len = 0;
        for (; i< num; i++) {
            struct TaskUnit* unit = new struct TaskUnit;
            int32 temp = 0;
            unit->task_id = in.Read64();
            unit->attr_id = in.Read64();
            unit->unix_time = in.Read64();
            unit->max_depth = in.Read8();
            unit->current_depth = in.Read8();
            unit->machine = in.Read8();
            unit->storage = in.Read8();
            unit->is_login = in.Read8();
            unit->is_over = in.Read8();
            unit->is_forge = in.Read8();
            unit->method = in.Read8();
            memcpy(unit->url, in.ReadData(URL_SIZE - 1, temp),
                    URL_SIZE - 1);
            int32 url_len = (temp - 1) < (URL_SIZE - 1) ?
                              (temp - 1) : (URL_SIZE - 1);
            unit->url[url_len] = '\0';

            vAssignmentDetailTask->task_set.push_back(unit);
        }
        break;
      }
      
      case REPLY_TASK_STATE : {
        struct ReplyTaskState* vReplyTaskState =
                new struct ReplyTaskState;

        *packet_head = (struct PacketHead*)vReplyTaskState;

        FILLHEAD();

        vReplyTaskState->id = in.Read32();
        int32 temp = 0;
        memcpy(vReplyTaskState->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                          (temp - 1) : (TOKEN_SIZE - 1);
        vReplyTaskState->token[token_len] = '\0';

        vReplyTaskState->jobid = in.Read64();
        vReplyTaskState->state = in.Read8();
        break;
      }
      
      case REPLY_DETAIL_STATE : {
        struct ReplyDetailState* vReplyDetailState =
                new struct ReplyDetailState;

        *packet_head = (struct PacketHead*)vReplyDetailState;

        FILLHEAD();

        vReplyDetailState->id = in.Read32();
        int32 temp = 0;
        memcpy(vReplyDetailState->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                          (temp - 1) : (TOKEN_SIZE - 1);
        vReplyDetailState->token[token_len] = '\0';

        vReplyDetailState->jobid = in.Read64();
        vReplyDetailState->state = in.Read8();
        break;
      }

      case GET_CRAWL_CONTENT_NUM : {
        struct CrawlContentNum* vCrawlContentNum = new struct CrawlContentNum;
        *packet_head = (struct CrawlContentNum*)vCrawlContentNum;

        FILLHEAD();

        vCrawlContentNum = new struct CrawlContentNum;
        vCrawlContentNum->id = in.Read32();
        vCrawlContentNum->jobid = in.Read64();
        break;
      }

      case REPLY_CRAWL_CONTENT_NUM : {
          struct ReplyCrawlContentNum* vReplyCrawlContentNum
                 = new ReplyCrawlContentNum;
          *packet_head = (struct ReplyCrawlContentNum*)vReplyCrawlContentNum;

          FILLHEAD();

          vReplyCrawlContentNum->id = in.Read32();
          int32 temp = 0;
          memcpy(vReplyCrawlContentNum->token,
                  in.ReadData(TOKEN_SIZE - 1, temp),
                  TOKEN_SIZE - 1);
          int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                            (temp - 1) : (TOKEN_SIZE - 1);
           vReplyCrawlContentNum->token[token_len] = '\0';


          vReplyCrawlContentNum->task_id = in.Read64();
          vReplyCrawlContentNum->num = in.Read32();

        break;
      }
      case CRAWL_HBASE_STORAGE_INFO :
      case CRAWL_FTP_STORAGE_INFO : {
        struct CrawlStorageInfo* vCrawlStorageInfo =
                new struct CrawlStorageInfo;
        *packet_head = (struct PacketHead*)vCrawlStorageInfo;

        FILLHEAD();

        vCrawlStorageInfo->id = in.Read32();

        int32 temp = 0;
        memcpy(vCrawlStorageInfo->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp) < (TOKEN_SIZE - 1) ?
                (temp) : (TOKEN_SIZE - 1);
        vCrawlStorageInfo->token[token_len] = '\0';


        int32 set_size = data_length - sizeof(int32) - (TOKEN_SIZE -1);
        int32 i = 0;
        int32 num = set_size / CRAWLSTORAGEINFO_SIZE;
        int32 len = 0;
        //LOG_DEBUG2("set_size = %d num = %d", set_size, num);
        for (; i< num; i++) {
            struct StorageUnit* unit = new struct StorageUnit;
            temp = 0;
            unit->task_id = in.Read64();
            unit->attr_id = in.Read64();
            unit->max_depth = in.Read8();
            unit->cur_depth = in.Read8();

            memcpy(unit->key_name, in.ReadData(STORAGE_INFO_SIZE - 1, temp),
                    STORAGE_INFO_SIZE - 1);

            int32 name_len = (temp) < (STORAGE_INFO_SIZE - 1) ?
                    (temp) : (STORAGE_INFO_SIZE - 1);
            unit->key_name[name_len] = '\0';
            //LOG_DEBUG2("task_id=%ld, attr_id=%ld, max_depth=%d, cur_depth=%d, key_name=%s temp=%d", unit->task_id, unit->attr_id, unit->max_depth, unit->cur_depth, unit->key_name, temp);


            temp = 0;
            memcpy(unit->pos_name, in.ReadData(URL_SIZE - 1, temp),
                    URL_SIZE - 1);
            int32 pos_len = (temp) < (URL_SIZE - 1) ?
                    (temp) : (URL_SIZE - 1);
            unit->pos_name[pos_len] = '\0';
            //LOG_DEBUG2("pos_name=%s temp=%d", unit->pos_name, temp);

            vCrawlStorageInfo->storage_set.push_back(unit);
        }
        break;
      }

      case GET_FORGEINFO : {
        struct GetForgeInfo* vGetForgeInfo =
               new struct GetForgeInfo;
        *packet_head = (struct GetForgeInfo*)vGetForgeInfo;

        FILLHEAD();

        vGetForgeInfo->id = in.Read32();
        int32 temp = 0;
        memcpy(vGetForgeInfo->token, in.ReadData(TOKEN_SIZE - 1, temp),
                TOKEN_SIZE - 1);
        int32 token_len = (temp - 1) < (TOKEN_SIZE - 1) ?
                            (temp - 1) : (TOKEN_SIZE - 1);
        vGetForgeInfo->token[token_len] = '\0';

        vGetForgeInfo->task_id = in.Read64();
        vGetForgeInfo->forge_type = in.Read8();
        vGetForgeInfo->num = in.Read8();

        break;
      }

      case REPLY_FOGEINFO_UA: {
        struct ReplyUAForgeInfo* vReplyUAForgeInfo =
                  new struct ReplyUAForgeInfo;
        *packet_head = (struct PacketHead*)vReplyUAForgeInfo;

        FILLHEAD();

        vReplyUAForgeInfo->id = in.Read32();
        vReplyUAForgeInfo->task_id = in.Read64();

        int32 set_size = data_length - sizeof(int32) - sizeof(int64);
        int32 i = 0;
        int32 num = set_size / (UA_FORGEN_SIZE - 1 + sizeof(int32) + sizeof(8));
        int32 len = 0;
        for (; i< num; i++) {
            struct UAForgeInfo* unit = new struct UAForgeInfo;
            int32 temp = 0;
            unit->id = in.Read32();
            unit->type = in.Read8();
            memcpy(unit->forgen_info, in.ReadData(UA_FORGEN_SIZE -1, temp),
                    UA_FORGEN_SIZE - 1);
            int32 ua_forgen_len = (temp - 1) < (UA_FORGEN_SIZE - 1) ?
                               (temp - 1) : (UA_FORGEN_SIZE - 1);
            unit->forgen_info[ua_forgen_len] = '\0';


            vReplyUAForgeInfo->forgen_set.push_back(unit);
        }
        break;
      }
      case REPLY_FOGEINFO_IP: {
        struct ReplyIPForgeInfo* vReplyIPForgeInfo =
                new struct ReplyIPForgeInfo;
        *packet_head = (struct PacketHead*)vReplyIPForgeInfo;

        FILLHEAD();

        vReplyIPForgeInfo->id = in.Read32();
        vReplyIPForgeInfo->task_id = in.Read64();

        int32 set_size = data_length - sizeof(int32) - sizeof(int64);
        int32 i = 0;
        int32 num = set_size / (IP_FORGEN_SIZE - 1 + sizeof(int32) + sizeof(8));
        int32 len = 0;
        for (; i< num; i++) {
            struct IPForgeInfo* unit = new struct IPForgeInfo;
            int32 temp = 0;
            unit->id = in.Read32();
            unit->type = in.Read8();
            memcpy(unit->forgen_info, in.ReadData(IP_FORGEN_SIZE -1, temp),
                    IP_FORGEN_SIZE - 1);
            int32 ip_forgen_len = (temp - 1) < (IP_FORGEN_SIZE - 1) ?
                                (temp - 1) : (IP_FORGEN_SIZE - 1);
             unit->forgen_info[ip_forgen_len] = '\0';

            vReplyIPForgeInfo->forgen_set.push_back(unit);
        }
        break;
      }

      case ANALYTICAL_INFO: {
          struct AnalysiInfo* vAnalysiInfo =
                  new struct AnalysiInfo;
          *packet_head = (struct PacketHead*)vAnalysiInfo;
          FILLHEAD();

          int32 set_size = data_length;
          int32 i = 0;
          int32 num = set_size / ANALYIUNIT_SIZE;
          int32 len = 0;
          for (; i< num; i++) {
              struct AnalysiUnit* unit = new struct AnalysiUnit;
              int32 temp = 0;
              unit->ayalysi_id = in.Read64();
              unit->task_id = in.Read64();
              unit->attr_id = in.Read32();
              unit->type = in.Read8();
              unit->max_depth = in.Read8();
              unit->cur_depth = in.Read8();
              memcpy(unit->name, in.ReadData(NAME_SIZE - 1, temp),
                      NAME_SIZE - 1);
              int32 name_len = (temp - 1) < (NAME_SIZE - 1) ?
                                 (temp - 1) : (NAME_SIZE - 1);
              unit->name[name_len] = '\0';

              memcpy(unit->key, in.ReadData(KEY_SIZE - 1, temp),
                       KEY_SIZE - 1);
              int32 key_len = (temp - 1) < (KEY_SIZE - 1) ?
                                 (temp - 1) : (KEY_SIZE - 1);
              unit->key[key_len] = '\0';


              vAnalysiInfo->analysi_set.push_back(unit);
          }
          break;
      }

      case ANALYTICAL_URL_SET: {
          struct AnalyticalURLSet* vAnalyticalURLSet =
                  new struct AnalyticalURLSet;
          *packet_head = (struct PacketHead*)vAnalyticalURLSet;
          FILLHEAD();

          int32 set_size = data_length - sizeof(int32) - (TOKEN_SIZE - 1);
          int32 i = 0;
          int32 num = set_size / ANALYTICAL_URL_UNIT_SIZE;
          int32 len = 0;

          vAnalyticalURLSet->id = in.Read32();
          int32 temp = 0;
          memcpy(vAnalyticalURLSet->token, in.ReadData(TOKEN_SIZE - 1, temp),
                  TOKEN_SIZE - 1);
          int32 token_len = (temp) < (TOKEN_SIZE - 1) ?
                              (temp) : (TOKEN_SIZE - 1);
          vAnalyticalURLSet->token[token_len] = '\0';

          for (; i< num; i++) {
              struct AnalyticalURLUnit* unit = new struct AnalyticalURLUnit;
              int32 temp = 0;
              unit->task_id = in.Read64();
              unit->attr_id = in.Read64();
              unit->max_depth = short(in.Read8());
              unit->current_depth = short(in.Read8());
              unit->method = short(in.Read8());

              memcpy(unit->url, in.ReadData(URL_SIZE - 1, temp),
                      URL_SIZE - 1);
              int32 url_len = (temp - 1) < (URL_SIZE - 1) ?
                                 (temp - 1) : (URL_SIZE - 1);
              unit->url[url_len] = '\0';

              vAnalyticalURLSet->analytical_url_set.push_back(unit);
          }
          break;
      }
      case ANALYTICAL_STATE: {
        struct AnalysiState* vAnalysiState =
                new struct AnalysiState;
        *packet_head = (struct PacketHead*)vAnalysiState;

        FILLHEAD();
        vAnalysiState->id = in.Read64();
        vAnalysiState->state = in.Read8();
        break;
      }

      case LOGIN_REQUIRE_COOKIES: {

        //LOG_DEBUG2("%s","begin to parse request!");
        struct RequireLoginCookie* vRequireLoginCookie =
            new struct RequireLoginCookie;
        *packet_head = (struct PacketHead*)vRequireLoginCookie;

        FILLHEAD();

        vRequireLoginCookie->manage_id = in.Read32();
        int temp = 0;
        memcpy(vRequireLoginCookie->token, in.ReadData(TOKEN_SIZE -1, temp),
             TOKEN_SIZE -1);
        int token_len = (temp -1) < (TOKEN_SIZE -1) ?
                (temp -1):(TOKEN_SIZE -1);
        vRequireLoginCookie->token[token_len] = '\0';
        vRequireLoginCookie->attr_id = in.Read64();
        vRequireLoginCookie->amount = in.Read8();
        break;
      }

      case CRAWLER_AVAILABLE_RESOURCE_NUM: {

		  struct CrawlerAvailableResourceNum* vCrawlerAvailableResource =
			  new struct CrawlerAvailableResourceNum;
		  *packet_head = (struct PacketHead*)vCrawlerAvailableResource;
		  FILLHEAD();
		  vCrawlerAvailableResource->manage_id = in.Read32();
		  vCrawlerAvailableResource->task_num = in.Read16();
		  //LOG_DEBUG2("vCrawlerAvailableResource->task_num = %d", (int)vCrawlerAvailableResource->task_num);
		  break;
      }

      case HEART_PACKET:
      case GET_HARDINFO:
      case TEMP_CRAWLER_OP:{
          struct PacketHead* vHead =
                  new struct PacketHead;
          *packet_head = (struct PacketHead*)vHead;
          FILLHEAD();
          break;
      }
      default:
        r = false;
        break;
    }*/
    return r;
}

void PacketProsess::DeletePacket(const void* packet_stream, int32 len,
            struct PacketHead* packet_head) {
    packet::DataInPacket in_packet(reinterpret_cast<const char*>(packet_stream),
            len);
    int16 packet_length = in_packet.Read16();
    int8 is_zip_encrypt = in_packet.Read8();
    char* body_stream = NULL;
    int32 body_length = 0;
    int32 temp = 0;

    // 是否解压解码
    if (is_zip_encrypt == ZIP_AND_NOENCRYPT) {
        char* temp_body_stream =
                reinterpret_cast<char*>(const_cast<void*>(packet_stream))
                + sizeof(int16) + sizeof(int8);
        const uint8* zipdata = reinterpret_cast<uint8*>(temp_body_stream);
        uint8* unzipdata = NULL;
        int32 temp_body_len = len - sizeof(int16) - sizeof(int8);
        body_length = DecompressionStream(zipdata,
                temp_body_len,
                &unzipdata);
        body_stream = reinterpret_cast<char*>(unzipdata);
    } else {
        body_stream =
                reinterpret_cast<char*>(const_cast<void*>(packet_stream)) +
                sizeof(int16) + sizeof(int8);
        body_length = len - sizeof(int16) - sizeof(int8);
    }


    BUILDPACKHEAD();

    switch (operate_code) {
      case CRAWLER_MGR_REG:
      case ANALYTICAL_REG: {
        struct CrawlerMgrReg* vCrawlerMgrReg = (struct CrawlerMgrReg*)packet_head;
        delete vCrawlerMgrReg;
       break;
      }

      case CRAWLER_MGR_REGSTATE: {
        struct CrawlerMgrRegState* vCrawlerMgrRegState = (struct CrawlerMgrRegState*)packet_head;
        delete vCrawlerMgrRegState;
         break;
      }

      case CRAWLER_REG_FAILED: {
          struct CrawlerFailed* vCrawlerFailed = (struct CrawlerFailed*)packet_head;
          delete vCrawlerFailed;
          break;
      }

      case REPLY_HARDINFO: {
        struct ReplyHardInfo* vReplyHardInfo = (struct ReplyHardInfo*)packet_head;
        delete vReplyHardInfo;
         break;
      }

      case ASSIGNMENT_SINGLE_TASK: {
        struct AssignmentSingleTask* vAssignmentSingleTask = (struct AssignmentSingleTask*)packet_head;
        delete vAssignmentSingleTask;
        break;
      }

      case ASSIGNMENT_MULTI_TASK : {
        struct AssignmentMultiTask* vAssignmentMultiTask = (struct AssignmentMultiTask*)packet_head;
        delete vAssignmentMultiTask;
        break;
      }
      
      case ASSIGNMENT_DETAIL_TASK : {
        struct AssignmentDetailTask* vAssignmentDetailTask = (struct AssignmentDetailTask*)packet_head;
        delete vAssignmentDetailTask;
        break;
      }

      case REPLY_TASK_STATE : {
        struct ReplyTaskState* vReplyTaskState = (struct ReplyTaskState*)packet_head;
        delete vReplyTaskState;
        break;
      }

      case REPLY_DETAIL_STATE : {
        struct ReplyDetailState* vReplyDetailState = (struct ReplyDetailState*)packet_head;
        delete vReplyDetailState;
        break;
      }
      
      case GET_CRAWL_CONTENT_NUM : {
        struct CrawlContentNum* vCrawlContentNum = (struct CrawlContentNum*)packet_head;
        delete vCrawlContentNum;
        break;
      }

      case REPLY_CRAWL_CONTENT_NUM : {
          struct ReplyCrawlContentNum* vReplyCrawlContentNum = (struct ReplyCrawlContentNum*)packet_head;
          delete vReplyCrawlContentNum;
          break;
      }
      case CRAWL_HBASE_STORAGE_INFO :
      case CRAWL_FTP_STORAGE_INFO : {
        struct CrawlStorageInfo* vCrawlStorageInfo = (struct CrawlStorageInfo*)packet_head;
        delete vCrawlStorageInfo;
        break;
      }

      case GET_FORGEINFO : {
        struct GetForgeInfo* vGetForgeInfo = (struct GetForgeInfo*)packet_head;
        delete vGetForgeInfo;
        break;
      }

      case REPLY_FOGEINFO_UA: {
        struct ReplyUAForgeInfo* vReplyUAForgeInfo = (struct ReplyUAForgeInfo*)packet_head;
        delete vReplyUAForgeInfo;
        break;
      }
      case REPLY_FOGEINFO_IP: {
        struct ReplyIPForgeInfo* vReplyIPForgeInfo = (struct ReplyIPForgeInfo*)packet_head;
        delete vReplyIPForgeInfo;
        break;
      }

      case ANALYTICAL_INFO: {
          struct AnalysiInfo* vAnalysiInfo = (struct AnalysiInfo*)packet_head;
          delete vAnalysiInfo;
          break;
      }

      case ANALYTICAL_URL_SET: {
          struct AnalyticalURLSet* vAnalyticalURLSet = (struct AnalyticalURLSet*)packet_head;
          delete vAnalyticalURLSet;
          break;
      }
      case ANALYTICAL_STATE: {
        struct AnalysiState* vAnalysiState = (struct AnalysiState*)packet_head;
        delete vAnalysiState;
        break;
      }

      case LOGIN_REQUIRE_COOKIES: {

        //LOG_DEBUG2("%s","begin to parse request!");
        struct RequireLoginCookie* vRequireLoginCookie = (struct RequireLoginCookie*)packet_head;
        delete vRequireLoginCookie;
        break;
      }

      case CRAWLER_AVAILABLE_RESOURCE_NUM: {

		  struct CrawlerAvailableResourceNum* vCrawlerAvailableResource =
			  (struct CrawlerAvailableResourceNum*)packet_head;
		  delete vCrawlerAvailableResource;
		  break;
      }

      case HEART_PACKET:
      case GET_HARDINFO:
      case TEMP_CRAWLER_OP:{
          struct PacketHead* vHead = (struct PacketHead*)packet_head;
          delete vHead;
          break;
      }
      default:
        break;
    }
}


void PacketProsess::DumpPacket(const struct PacketHead* packet_head) {
#if defined DUMP_PACKET
    int16 packet_length = packet_head->packet_length;
    int8 is_zip_encrypt = packet_head->is_zip_encrypt;
    int8 type = packet_head->type;
    int16 operate_code = packet_head->operate_code;
    int16 data_length = packet_head->data_length;
    int32 timestamp = packet_head->timestamp;
    int64 session_id = packet_head->session_id;
    int32 reserved = packet_head->reserved;
    int16 signature = packet_head->signature;

    char buf[DUMPPACKBUF];
    bool r = false;
    int32 j = 0;
/*
    switch (operate_code) {
      case CRAWLER_MGR_REG :
      case ANALYTICAL_REG : {
        struct CrawlerMgrReg* vCrawlerMgrReg =
                (struct CrawlerMgrReg*)packet_head;
        PRINT_TITLE("struct CrawlerMgrReg Dump Begin");
        DUMPHEAD();
        PRINT_INT(vCrawlerMgrReg->level);
        PRINT_STRING(vCrawlerMgrReg->password);
        PRINT_STRING(vCrawlerMgrReg->mac);
        PRINT_END("struct CrawlerMgrReg Dump End");
        break;
      }

      case CRAWLER_MGR_REGSTATE : {
        struct CrawlerMgrRegState* vCrawlerMgrRegState =
                (struct CrawlerMgrRegState*)packet_head;
        PRINT_TITLE("struct CrawlerMgrRegState Dump Begin");
        DUMPHEAD();
        PRINT_INT(vCrawlerMgrRegState->id);
        PRINT_STRING(vCrawlerMgrRegState->token);
        PRINT_END("struct CrawlerMgrRegState Dump End");
        break;
      }

      case CRAWLER_REG_FAILED : {
          struct CrawlerFailed* vCrawlerFailed =
                  (struct CrawlerFailed*)packet_head;
          PRINT_TITLE("struct CrawlerFailed Dump Begin");
          DUMPHEAD();
          PRINT_INT(vCrawlerFailed->erron_code);
          PRINT_END("struct CrawlerFailed Dump End");
          break;
      }

      case REPLY_HARDINFO : {
        struct ReplyHardInfo* vReplyHardInfo =
                (struct ReplyHardInfo*)packet_head;
        PRINT_TITLE("struct ReplyHardInfo Dump Begin");
        DUMPHEAD();
        PRINT_INT(vReplyHardInfo->id);
        PRINT_STRING(vReplyHardInfo->token);
        PRINT_STRING(vReplyHardInfo->cpu);
        PRINT_STRING(vReplyHardInfo->mem);
        PRINT_END("struct ReplyHardInfo Dump End");
        break;
      }

      case ASSIGNMENT_SINGLE_TASK : {
        struct AssignmentSingleTask* vAssignmentSingleTask =
                (struct AssignmentSingleTask*)packet_head;
        PRINT_TITLE("struct AssignmentSingleTask Dump Begin");
        DUMPHEAD();
        PRINT_INT(vAssignmentSingleTask->id);
        PRINT_INT64(vAssignmentSingleTask->task_id);
        PRINT_INT(vAssignmentSingleTask->depth);
        PRINT_INT(vAssignmentSingleTask->machine);
        PRINT_INT(vAssignmentSingleTask->storage);
        PRINT_END("struct AssignmentSingleTask Dump End");
        break;
      }
      case ASSIGNMENT_MULTI_TASK : {
        struct AssignmentMultiTask* vAssignmentMultiTask =
                (struct AssignmentMultiTask*)packet_head;
        std::list<struct TaskUnit*>::iterator it =
                vAssignmentMultiTask->task_set.begin();
        PRINT_TITLE("struct AssignmentMultiTask Dump Begin");
        DUMPHEAD();
        PRINT_INT(vAssignmentMultiTask->id);
        for (; it != vAssignmentMultiTask->task_set.end(); it++) {
            PRINT_INT64((*it)->task_id);
            PRINT_INT64((*it)->pid);
            PRINT_INT64((*it)->attr_id);
            PRINT_INT64((*it)->unix_time);
            PRINT_INT64((*it)->max_depth);
            PRINT_INT64((*it)->current_depth);
            PRINT_INT64((*it)->machine);
            PRINT_INT64((*it)->storage);
            PRINT_INT64((*it)->is_login);
            PRINT_INT64((*it)->is_over);
            PRINT_INT64((*it)->is_forge);
            PRINT_INT64((*it)->method);
            PRINT_STRING((*it)->url);
        }
        PRINT_END("struct AssignmentDetailTask Dump End");
        break;
      }
      case ASSIGNMENT_DETAIL_TASK : {
        struct AssignmentDetailTask* vAssignmentDetailTask =
                (struct AssignmentDetailTask*)packet_head;
        std::list<struct TaskUnit*>::iterator it =
                vAssignmentDetailTask->task_set.begin();
        PRINT_TITLE("struct AssignmentDetailTask Dump Begin");
        DUMPHEAD();
        PRINT_INT(vAssignmentDetailTask->id);
        for (; it != vAssignmentDetailTask->task_set.end(); it++) {
            PRINT_INT64((*it)->task_id);
            PRINT_INT64((*it)->attr_id);
            PRINT_INT64((*it)->unix_time);
            PRINT_INT64((*it)->max_depth);
            PRINT_INT64((*it)->current_depth);
            PRINT_INT64((*it)->machine);
            PRINT_INT64((*it)->storage);
            PRINT_INT64((*it)->is_login);
            PRINT_INT64((*it)->is_over);
            PRINT_INT64((*it)->is_forge);
            PRINT_INT64((*it)->method);
            PRINT_STRING((*it)->url);
        }
        PRINT_END("struct AssignmentDetailTask Dump End");
        break;
      }
      case REPLY_TASK_STATE : {
        struct ReplyTaskState* vReplyTaskState =
                (struct ReplyTaskState*)packet_head;
        PRINT_TITLE("struct ReplyTaskState Dump Begin");
        DUMPHEAD();
        PRINT_INT(vReplyTaskState->id);
        PRINT_STRING(vReplyTaskState->token);
        PRINT_INT(vReplyTaskState->jobid);
        PRINT_INT(vReplyTaskState->state);
        PRINT_END("struct ReplyTaskState Dump End");
        break;
      }
      case REPLY_DETAIL_STATE : {
        struct ReplyDetailState* vReplyDetailState =
                (struct ReplyDetailState*)packet_head;
        PRINT_TITLE("struct ReplyDetailState Dump Begin");
        DUMPHEAD();
        PRINT_INT(vReplyDetailState->id);
        PRINT_STRING(vReplyDetailState->token);
        PRINT_INT(vReplyDetailState->jobid);
        PRINT_INT(vReplyDetailState->state);
        PRINT_END("struct ReplyDetailState Dump End");
        break;
      }
      case GET_CRAWL_CONTENT_NUM : {
        struct CrawlContentNum* vCrawlContentNum =
                (struct CrawlContentNum*)packet_head;
        PRINT_TITLE("struct CrawlContentNum Dump Begin");
        DUMPHEAD();
        PRINT_INT(vCrawlContentNum->id);
        PRINT_INT64(vCrawlContentNum->jobid);
        PRINT_END("struct CrawlContentNum Dump End");
        break;
      }
      case REPLY_CRAWL_CONTENT_NUM : {
        struct ReplyCrawlContentNum* vReplyCrawlContentNum =
                (struct ReplyCrawlContentNum*)packet_head;
        PRINT_TITLE("struct ReplyCrawlContentNum Dump Begin");
        PRINT_INT(vReplyCrawlContentNum->id);
        PRINT_STRING(vReplyCrawlContentNum->token);
        PRINT_INT64(vReplyCrawlContentNum->task_id);
        PRINT_INT(vReplyCrawlContentNum->num);
        PRINT_END("struct ReplyCrawlContentNum Dump End");
        break;
      }
      case CRAWL_HBASE_STORAGE_INFO :
      case CRAWL_FTP_STORAGE_INFO : {
        struct CrawlStorageInfo* vCrawlStorageInfo =
                (struct CrawlStorageInfo*)packet_head;
        std::list<struct StorageUnit*>::iterator it =
                vCrawlStorageInfo->storage_set.begin();
        PRINT_TITLE("struct CrawlStorageInfo Dump Begin");
        DUMPHEAD();
        PRINT_INT(vCrawlStorageInfo->id);
        PRINT_STRING(vCrawlStorageInfo->token);
        for (; it != vCrawlStorageInfo->storage_set.end(); it++) {
            PRINT_INT64((*it)->task_id);
            PRINT_INT64((*it)->attr_id);
            PRINT_STRING((*it)->key_name);
            PRINT_STRING((*it)->pos_name);
        }
        PRINT_END("struct CrawlStorageInfo Dump End");
        break;
      }
      case GET_FORGEINFO : {
        struct GetForgeInfo* vGetForgeInfo =
                (struct GetForgeInfo*)packet_head;
        PRINT_TITLE("struct GetForgeInfo Dump Begin");
        DUMPHEAD();
        PRINT_INT(vGetForgeInfo->id);
        PRINT_STRING(vGetForgeInfo->token);
        PRINT_INT64(vGetForgeInfo->task_id);
        PRINT_INT(vGetForgeInfo->forge_type);
        PRINT_INT(vGetForgeInfo->num);
        PRINT_END("struct GetForgeInfo Dump End");
        break;
      }

      case REPLY_FOGEINFO_IP:  {
        struct ReplyIPForgeInfo* vReplyForgeInfo =
                  (struct ReplyIPForgeInfo*)packet_head;
        std::list<struct IPForgeInfo*>::iterator it =
                  vReplyForgeInfo->forgen_set.begin();
        PRINT_TITLE("struct ReplyIPForgeInfo Dump Begin");
        DUMPHEAD();
        PRINT_INT(vReplyForgeInfo->id);
        PRINT_INT64(vReplyForgeInfo->task_id);
        for (; it != vReplyForgeInfo->forgen_set.end(); it++) {
            PRINT_INT((*it)->id);
            PRINT_INT((*it)->type);
            PRINT_STRING((*it)->forgen_info);
        }
        PRINT_END("struct ReplyIPForgeInfo Dump End");
        break;
      }
      case REPLY_FOGEINFO_UA:  {
        struct ReplyUAForgeInfo* vReplyForgeInfo =
                  (struct ReplyUAForgeInfo*)packet_head;
        std::list<struct UAForgeInfo*>::iterator it =
                  vReplyForgeInfo->forgen_set.begin();
        PRINT_TITLE("struct ReplyUAForgeInfo Dump Begin");
        DUMPHEAD();
        PRINT_INT(vReplyForgeInfo->id);
        PRINT_INT64(vReplyForgeInfo->task_id);
        for (; it != vReplyForgeInfo->forgen_set.end(); it++) {
            PRINT_INT((*it)->id);
            PRINT_INT((*it)->type);
            PRINT_STRING((*it)->forgen_info);
        }
        PRINT_END("struct ReplyUAForgeInfo Dump End");
        break;
      }
      case ANALYTICAL_INFO:  {
        struct AnalysiInfo* vAnalysiInfo =
                  (struct AnalysiInfo*)packet_head;
        std::list<struct AnalysiUnit*>::iterator it =
                vAnalysiInfo->analysi_set.begin();
        PRINT_TITLE("struct AnalysiInfo Dump Begin");
        DUMPHEAD();
        for (; it != vAnalysiInfo->analysi_set.end(); it++) {
            PRINT_INT64((*it)->ayalysi_id);
            PRINT_INT64((*it)->task_id);
            PRINT_INT((*it)->attr_id);
            PRINT_INT((*it)->type);
            PRINT_INT((*it)->cur_depth);
            PRINT_INT((*it)->max_depth);
            PRINT_STRING((*it)->name);
            PRINT_STRING((*it)->key);
        }
        PRINT_END("struct AnalysiInfo Dump End");
        break;
      }
      case ANALYTICAL_STATE : {
        struct AnalysiState* vAnalysiState =
                (struct AnalysiState*)packet_head;
        PRINT_TITLE("struct AnalysiState Dump Begin");
        DUMPHEAD();
        PRINT_INT64(vAnalysiState->id);
        PRINT_INT(vAnalysiState->state);
        PRINT_END("struct AnalysiState Dump End");
        break;
      }
/*      case ANALYTICAL_URL_SET : {
        struct AnalyticalURLSet* vAnalyticalURLSet =
                (struct AnalyticalURLSet*)packet_head;
        std::list<struct AnalyticalURLUnit*>::iterator it =
                vAnalyticalURLSet->analytical_url_set.begin();
        PRINT_TITLE("struct AnalyticalURLSet Dump Begin");
        DUMPHEAD();
        PRINT_INT(vAnalyticalURLSet->id);
        PRINT_STRING(vAnalyticalURLSet->token);
        for (; it != vAnalyticalURLSet->analytical_url_set.end(); it++) {
            PRINT_INT64((*it)->task_id);
            PRINT_INT64((*it)->attr_id);
            PRINT_INT64((*it)->max_depth);
            PRINT_INT64((*it)->current_depth);
            PRINT_INT64((*it)->method);
            PRINT_STRING((*it)->url);
        }
        PRINT_END("struct AnalyticalURLSet Dump End");
        break;
      }*/

      case HEART_PACKET:
      case GET_HARDINFO:
      case TEMP_CRAWLER_OP: {
        PRINT_TITLE("struct PacketHead Dump Begin");
        DUMPHEAD();
        PRINT_END("struct PacketHead Dump End");
      }
      default:
        break;
    }*/
    if (buf[0] != '\0')
            LOG_DEBUG2("%s\n", buf);
#endif
}

void PacketProsess::HexEncode(const void *bytes, size_t size) {
#if defined HEXDUMP
    struct PacketHead* head = (struct PacketHead*)bytes;
    static const char kHexChars[] = "0123456789ABCDEF";
    std::string sret(size*3, '\0');
    for (size_t i = 0; i < size; ++i) {
        char b = reinterpret_cast<const char*>(bytes)[i];
        sret[(i*3)] = kHexChars[(b>>4) & 0xf];
        sret[(i*3)+1] = kHexChars[b&0xf];
        if ((((i*3)+2+1)%12) != 0)
            sret[(i * 3) + 2] = '\40';
        else
            sret[(i * 3) + 2] = '\n';
    }
    LOG_DEBUG2("===start====\nopcode[%d]:\n%s\n====end====\n",
                head->operate_code, sret.c_str());
#endif

}

uint64 PacketProsess::CompressionStream(const uint8* unzip_data,
        uint64 unzip_len, uint8** zip_data) {
    MZip zip_engine;
    unzip_len = zip_engine.ZipData(unzip_data, unzip_len, zip_data);
    return unzip_len;
}

uint64 PacketProsess::DecompressionStream(const uint8* zip_data,
        uint64 zip_len, uint8** unzip_data) {
    MUnZip unzip_engine;
    zip_len = unzip_engine.UnZipData(zip_data, zip_len, unzip_data);
    return zip_len;
}



}  //  namespace net
