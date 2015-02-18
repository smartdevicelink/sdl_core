/* Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_GENERATED_CODE_WITH_SQLITE_TEST_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_GENERATED_CODE_WITH_SQLITE_TEST_H_

#include <string>
#include "./types.h"
#include "rpc_base/rpc_base.h"
#include "sqlite_wrapper/sql_query.h"
#include "sqlite_wrapper/sql_database.h"

namespace policy_table = rpc::policy_table_interface_base;
namespace dbms = utils::dbms;

namespace rpc {
namespace policy_table_interface_base {

bool FindSection(dbms::SQLDatabase* db, policy_table::ServiceEndpoints& ep) {
  /*
   * Following table structure is assumed:
   *
   * table Endpoints
   * index, service_type, application_id, url, is_default
   *
   * If url belongs to default section, application_id should be null and is_defaut = true
   * Otherwise application_id should be set and is_default = false
   */

  std::string query = "select * from Endpoints";

  dbms::SQLQuery sqlquery(db);
  sqlquery.Prepare(query);
  if (!sqlquery.Exec()) {
    return false;
  }

  /*
   * Following query result is assumed (data from wp1_policy_table.json):
   * 1, 0x07, null, http://applinkqa.trafficmanager.net/api/policies, true
   */

  policy_table::URL urls;
  urls.push_back(sqlquery.GetString(3));

  policy_table::URLList urllist;
  if (sqlquery.GetBoolean(4)) {
    urllist["default"] = urls;
  } else {
    urllist[sqlquery.GetString(2)] = urls;
  }

  policy_table::ServiceEndpoints new_ep;
  new_ep[sqlquery.GetString(1)] = urllist;

  ep = new_ep;

  return true;
}

bool FindSection(dbms::SQLDatabase* db, policy_table::ModuleConfig& mc) {
  policy_table::ModuleConfig new_mc;
  FindSection(db, new_mc.endpoints);
  mc = new_mc;

  return true;
}

bool RemoveSection(dbms::SQLDatabase* db,
                   const policy_table::ApplicationPolicies& ap) {
  dbms::SQLQuery sqlquery(db);
  bool is_policies_removed = sqlquery.Exec("delete from AppPolicies");
  //      bool is_nicknames_removed = sqlquery.Exec("delete from Nicknames");
  bool is_groups_removed = sqlquery.Exec("delete from Groups");

  return is_policies_removed /*&& is_nicknames_removed*/&& is_groups_removed;
}

bool RemoveSection(dbms::SQLDatabase* db,
                   const policy_table::ServiceEndpoints& ep) {
  std::string query = "delete from Endpoints";
  dbms::SQLQuery sqlquery(db);
  return sqlquery.Exec(query);
}

bool RemoveSection(dbms::SQLDatabase* db,
                   const policy_table::ModuleConfig& mc) {
  //      std::string query = "delete * from ModuleConfig";
  //      sqlite::SQLQuery sqlquery(db);
  //      sqlquery.Exec(query);

  return RemoveSection(db, mc.endpoints);
}

bool RemoveSection(dbms::SQLDatabase* db,
                   const policy_table::FunctionalGroupings& fg) {
  std::string query = "delete from FunctionalGroups";
  dbms::SQLQuery sqlquery(db);
  return sqlquery.Exec(query);
}

bool UpdateSection(dbms::SQLDatabase* db,
                   const policy_table::ServiceEndpoints& ep) {
  /*
   * Following table structure is assumed:
   *
   * table Endpoints
   * index, service_type, application_id, url, is_default
   *
   * If url belongs to default section, application_id should be null and is_defaut = true
   * Otherwise application_id should be set and is_default = false
   */

  // According to documentation, we have to REPLACE this part on update coming,
  // so we delete all data first;
  if (!RemoveSection(db, ep)) {
    return false;
  }

  std::string query = "insert into Endpoints values(?,?,?,?,?)";
  dbms::SQLQuery sqlquery(db);
  sqlquery.Prepare(query);

  policy_table::ServiceEndpoints::const_iterator it_ep = ep.begin();
  policy_table::ServiceEndpoints::const_iterator it_ep_end = ep.end();

  // TODO: use define for int from stdint.h
  for (int index = 1; it_ep != it_ep_end; ++it_ep, ++index) {
    policy_table::URLList::const_iterator it_urllist = (*it_ep).second.begin();
    policy_table::URLList::const_iterator it_urllist_end =
        (*it_ep).second.end();

    for (; it_urllist != it_urllist_end; ++it_urllist) {
      policy_table::URL::const_iterator it_url = (*it_urllist).second.begin();
      policy_table::URL::const_iterator it_url_end = (*it_urllist).second.end();

      for (; it_url != it_url_end; ++it_url) {
        // Index binding
        sqlquery.Bind(0, index);

        // Service type binding
        sqlquery.Bind(1, (*it_ep).first);

        // Application_id and is_default binding
        std::string url_list_name = (*it_urllist).first;
        if ("default" == url_list_name) {
          sqlquery.Bind(2, "null");
          sqlquery.Bind(4, true);
        } else {
          sqlquery.Bind(2, url_list_name);
          sqlquery.Bind(4, false);
        }

        // URL binding
        sqlquery.Bind(3, (*it_url));

        if (sqlquery.Exec()) {
          sqlquery.Reset();
        } else {
          return false;
        }
      }
    }
  }

  return true;
}

bool UpdateSection(dbms::SQLDatabase* db,
                   const policy_table::ModuleConfig& mc) {
  UpdateSection(db, mc.endpoints);
  return true;
}

bool UpdateSection(dbms::SQLDatabase* db,
                   const policy_table::FunctionalGroupings& fg) {
  /*
   * Following table structure is assumed:
   *
   * table Rpcs - list of all available RPC commands
   * index, rpc
   *
   * table HmiLevels -list of all available hmi levels
   * index, hmi_level
   *
   * table Groups - list of functional group names
   * index, group_name
   *
   * table FunctionalGroups - list of functional groups
   * index, group_name_id, rpc_id, hmi_level_id
   *
   */

  // According to documentation, we have to REPLACE this part on update coming,
  // so we delete all data first;
  if (!RemoveSection(db, fg)) {
    return false;
  }

  std::string query = "insert into FunctionalGroups values("
      "?,"
      "(select index from Groups where group_name=?),"
      "(select index from Rpcs where rpc=?),"
      "(select index from HmiLevels where hmi_levels=?)";

  dbms::SQLQuery sqlquery(db);
  sqlquery.Prepare(query);

  policy_table::FunctionalGroupings::const_iterator it_fg = fg.begin();
  policy_table::FunctionalGroupings::const_iterator it_fg_end = fg.end();

  for (int index = 1; it_fg != it_fg_end; ++it_fg, ++index) {
    policy_table::Rpcs rpcs = (*it_fg).second;
    policy_table::Rpc::const_iterator it_rpcs = rpcs.rpcs.begin();
    policy_table::Rpc::const_iterator it_rpcs_end = rpcs.rpcs.end();

    for (; it_rpcs != it_rpcs_end; ++it_rpcs) {
      policy_table::RpcParameters rpc_params = (*it_rpcs).second;

      policy_table::HmiLevels::const_iterator it_hmi_levels = rpc_params
          .hmi_levels.begin();
      policy_table::HmiLevels::const_iterator it_hmi_levels_end = rpc_params
          .hmi_levels.end();

      for (; it_hmi_levels != it_hmi_levels_end; ++it_hmi_levels) {
        // Index binding
        sqlquery.Bind(0, index);

        // Group name binding
        sqlquery.Bind(1, (*it_fg).first);

        // RPC name binding
        sqlquery.Bind(2, (*it_rpcs).first);

        // Hmi levels binding
        sqlquery.Bind(3, (*it_hmi_levels));
        if (sqlquery.Exec()) {
          sqlquery.Reset();
        } else {
          return false;
        }
      }
    }
  }

  return true;
}

bool UpdateSection(dbms::SQLDatabase* db, const policy_table::Table& pt) {
  UpdateSection(db, pt.policy_table.module_config);
  UpdateSection(db, pt.policy_table.functional_groupings);
  return true;
}

bool UpdateSection(dbms::SQLDatabase* db,
                   const policy_table::ApplicationPolicies& ap) {
  /*
   * Following structure is assumed:
   *
   * table Groups - list of functional groups for application
   * index, application_id, functional_group_id
   *
   * table Nicknames - list of nickname for application
   * index, application_id, nickname
   *
   * table AppPolicies - policies for applications
   * index, application_id, priority, is_default
   *
   */

  // According to documentation, we have to REPLACE this part on update coming,
  // so we delete all data first;
  if (!RemoveSection(db, ap)) {
    return false;
  }

  std::string groups_query = "insert into Groups values ("
      "?,"
      "?,"
      "?)";
  dbms::SQLQuery groups_sqlquery(db);
  groups_sqlquery.Prepare(groups_query);

  std::string nicknames_query = "insert into Nicknames values(?,?,?)";
  dbms::SQLQuery nicknames_sqlquery(db);
  nicknames_sqlquery.Prepare(nicknames_query);

  std::string app_policies_query = "insert into AppPolicies values(?,?,?,?)";
  dbms::SQLQuery app_policies_sqlquery(db);
  app_policies_sqlquery.Prepare(app_policies_query);

  policy_table::ApplicationPolicies::const_iterator it_ap = ap.begin();
  policy_table::ApplicationPolicies::const_iterator it_ap_end = ap.end();

  for (int index = 0; it_ap != it_ap_end; ++it_ap, ++index) {
    // Index binding for AppPolicies table
    app_policies_sqlquery.Bind(0, index);

    std::string app_policy_name = (*it_ap).first;
    bool is_default_policy = "default" == app_policy_name ? true : false;
    if (is_default_policy) {
      app_policies_sqlquery.Bind(1, "null");
      app_policies_sqlquery.Bind(3, true);
    } else {
      app_policies_sqlquery.Bind(1, app_policy_name);
      app_policies_sqlquery.Bind(3, false);
    }

    // Struct contains groups, nicknames, priority for application/default section
    policy_table::ApplicationParams app_params = (*it_ap).second;

    // Priority binding
    app_policies_sqlquery.Bind(2, app_params.priority);
    app_policies_sqlquery.Bind(2, "Dummy priority parameter");

    // Add record to AppPolicies table
    if (app_policies_sqlquery.Exec()) {
      app_policies_sqlquery.Reset();
    } else {
      return false;
    }

    if (!is_default_policy) {
      // Seems, there is generator issue with Optional type inheritance
      // It should have pubic inheritance from type T to have array
      // begin/end methods in its interface
      // To be discussed with I.Kozyrenko

      //          policy_table::StringArray::const_iterator it_nicknames = app_params.nicknames.begin();
      //          policy_table::StringArray::const_iterator it_nicknames_end = app_params.nicknames.end();
      //
      //          for (int nick_index = 0;it_nicknames != it_nicknames_end; ++ it_nicknames, ++nick_index) {
      //            nicknames_sqlquery.Bind(0, nick_index);
      //            nicknames_sqlquery.Bind(1, app_policy_name);
      //
      //            nicknames_sqlquery.Bind(2, (*it_nicknames));
      //
      //            if (nicknames_sqlquery.Exec()) {
      //              nicknames_sqlquery.Reset();
      //            } else {
      //              return false;
      //            }
      //          }

      policy_table::Strings::const_iterator it_groups =
          app_params.groups.begin();
      policy_table::Strings::const_iterator it_groups_end = app_params.groups
          .end();

      for (int group_index = 0; it_groups != it_groups_end;
          ++it_groups, ++group_index) {
        groups_sqlquery.Bind(0, group_index);
        groups_sqlquery.Bind(1, app_policy_name);
        groups_sqlquery.Bind(2, (*it_groups));

        if (groups_sqlquery.Exec()) {
          groups_sqlquery.Reset();
        } else {
          return false;
        }
      }
    }
  }

  return true;
}

}  // namespace policy_table_interface_base
}  // namespace rpc

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_GENERATED_CODE_WITH_SQLITE_TEST_H_
