/**
 * Copyright (c) 2013, Ford Motor Company
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

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "application_manager/commands/hmi/on_vr_start_record_notification.h"
#include "application_manager/message_helper.h"
#include "application_manager/application_manager_impl.h"

#ifdef MODIFY_FUNCTION_SIGN
#include "config_profile/profile.h"
#include "utils/global.h"
#include <lib_msp_vr.h>
#include "utils/timer_thread.h"
#endif

namespace application_manager {

namespace commands {

OnVRStartRecordNotification::OnVRStartRecordNotification(
	const MessageSharedPtr& message) 
	: NotificationFromHMI(message)
{
	//vr_timer_ = new timer::TimerThread<OnVRStartRecordNotification>(this, &application_manager::commands::OnVRStartRecordNotification::OnTimer);
}

OnVRStartRecordNotification::~OnVRStartRecordNotification() {
	//delete vr_timer_;
}


void OnVRStartRecordNotification::Run() {
	LOG4CXX_INFO(logger_, "OnVRStartRecordNotification::Run");
	printf("OnVRStartRecordNotification::Run\n");
	ApplicationManagerImpl::instance()->set_vr_session_started(true);
#ifdef ASR_HAS_VR
	msp_vr_register_callback(this, OnVRStartRecordNotification::handleVRResult);
	RegisterAppCmdId();
	RegisterAppListId();
	
	msp_vr_grammar_construct(VRASSISTCOMMAND_CANCEL, VRASSISTCOMMAND_EXIT);
	msp_vrparm_set(MSP_VR_APP, 3000, 8000,16, 0);
	//if (vr_timer_==NULL)
	//	vr_timer_ = new timer::TimerThread<OnVRStartRecordNotification>(this, &application_manager::commands::OnVRStartRecordNotification::OnTimer);
	//vr_timer_->start(8);
	printf("record start\n");
	if (!msp_vr_start())
	{
		printf("record start failed\n");
		//vr_timer_->stop();
		//OnVRStartRecordNotification::handleVRResult(this, 0, NULL, -1);
	}
#endif
	
	/*if (0 != pthread_create(&thread_, 0, &RecordStart, NULL)) {
		LOG4CXX_INFO(logger_, "Create thread fail.");
	}*/
  
	//////////////////// Send notification to HMI /////////////////////
// 	MessageHelper::SendVRStatusToHMI("SUCCESS");
// 	MessageHelper::SendVRCancelToHMI();
// 	MessageHelper::SendVRCommandHelpToHMI();
// 	MessageHelper::SendVRCommandTTSToHMI();
// 	MessageHelper::SendVRExitAppToHMI();
// 	MessageHelper::SendVRSwitchAppToHMI(32768);
	
	//////////////////// Voice recognation /////////////////////////////
// 	ApplicationManagerImpl::instance()->handleVRCommand("VrCommand3");
}

    
#ifdef ASR_HAS_VR
void OnVRStartRecordNotification::handleVRResult(void *data,int cmdId, char *result, int code)
{
	
	//msp_vr_set_state(MSP_RECORD_END);
	if (code == 0 && result != NULL)
	{
		printf("******************************\ncommand id=%d,command text=%s\n******************************\n", cmdId, result);

		LOG4CXX_INFO(logger_, "VR result send sucess");
		OnVRStartRecordNotification  *object = static_cast<OnVRStartRecordNotification*>(data);
		
		VRStatus status = ApplicationManagerImpl::instance()->handleVRCommand(cmdId, result);
		printf("status=%d\n", status);
		if (VRSTATUS_SUCCESS == status)
		{
			MessageHelper::SendVRStatusToHMI("SUCCESS");
		}
		else if (VRSTATUS_FAIL == status)
		{
			MessageHelper::SendVRStatusToHMI("FAIL");
		}
		else
		{
			// do nothing
		}
	}
	else
	{
		LOG4CXX_INFO(logger_, "VR result send faild:" << "code=" << code);
		printf("vr send failed,code=%d\n", code);
		if (code <0)
		{

		}
		else if (code == 10114)
		{
			MessageHelper::SendVRStatusToHMI("TIME_OVER");
			printf("Timer out\n");
			return;
		}
		else
		{
			printf("vr send failed,code=%d\n", code);
			MessageHelper::SendVRStatusToHMI("FAIL");
		}
		
		/*OnVRStartRecordNotification  *object = static_cast<OnVRStartRecordNotification*>(data);
		if (object != NULL)
		{
			printf("stop timer\n");
			object->StopTimer();
		}*/
	}
}

//void  OnVRStartRecordNotification::StopTimer()
//{
//	vr_timer_->stop();
//}
//
//void OnVRStartRecordNotification::OnTimer()
//{
//	msp_vr_terminate();
//	MessageHelper::SendVRStatusToHMI("TIME_OVER");
//	printf("timer stop,Timer out\n");
//}


void OnVRStartRecordNotification::RegisterAppCmdId()
{
	ApplicationSharedPtr active_app = ApplicationManagerImpl::instance()->active_application();
	if (active_app){
		const application_manager::CommandsMap cmds = active_app->commands_map();

		for (CommandsMap::const_iterator command_it = cmds.begin();
			cmds.end() != command_it; ++command_it) {
			if ((*command_it->second).keyExists(strings::vr_commands)) {
				if ((*command_it->second)[strings::vr_commands]==0)
					continue;
				const smart_objects::SmartArray* vr_commands = (*command_it->second)[strings::vr_commands].asArray();
				smart_objects::SmartArray::const_iterator it_array =
					vr_commands->begin();
				smart_objects::SmartArray::const_iterator it_array_end =
					vr_commands->end();
				char cmd[200] = {0};
				if (ConstructedVRGrammars(cmd, it_array, it_array_end)>0){
					char appcmd[200] = {0};
					sprintf(appcmd, "(%s)", cmd);
					msp_vr_grammar_addcmd(appcmd, command_it->first);
				}
			}
		}
		//app help
		char help[200] = { 0 };
		if (!active_app->vr_synonyms())
			return;
		const smart_objects::SmartArray* vr_synonyms = active_app->vr_synonyms()->asArray();
		smart_objects::SmartArray::const_iterator it_array =
			vr_synonyms->begin();
		smart_objects::SmartArray::const_iterator it_array_end =
			vr_synonyms->end();
		if (ConstructedVRGrammars(help, it_array, it_array_end)){
			char helpen[500] = { 0 };
			char quien[500] = { 0 };
			sprintf(helpen, "((%s)(°ïÖú|help))", help);
			sprintf(quien, "($vr_exit(%s))", help);
			msp_vr_grammar_addcmd(helpen, VRASSISTCOMMAND_HELP + active_app->app_id());
			msp_vr_grammar_addcmd(quien, VRASSISTCOMMAND_EXIT + active_app->app_id());
		}
		
	}
}



void OnVRStartRecordNotification::RegisterAppListId()
{
	const std::set<ApplicationSharedPtr>& apps = ApplicationManagerImpl::instance()
		->applications();

	for (std::set<ApplicationSharedPtr>::const_iterator it = ApplicationManagerImpl::instance()->applications().begin();
		it != ApplicationManagerImpl::instance()->applications().end(); it++){
		if ((*it)->vr_synonyms()) {
			const smart_objects::SmartArray* vr_synonyms = (*it)->vr_synonyms()->asArray();
			smart_objects::SmartArray::const_iterator it_array =
				vr_synonyms->begin();
			smart_objects::SmartArray::const_iterator it_array_end =
				vr_synonyms->end();
			//Ó¦ÓÃ
			char cmd[200] = {0};
			if (ConstructedVRGrammars(cmd, it_array, it_array_end) > 0){
				char appcmd[200] = { 0 };
				sprintf(appcmd, "(%s)", cmd);
				msp_vr_grammar_addcmd(appcmd, ((*it)->app_id() + profile::Profile::instance()->max_cmd_id()));
			}

			
		}
	}
	
}


int OnVRStartRecordNotification::ConstructedVRGrammars(char cmd[],smart_objects::SmartArray::const_iterator  it_array, smart_objects::SmartArray::const_iterator it_array_end)
{
	int num = 0;
	for (; it_array != it_array_end; ++it_array) {
		if (num == 0){
			std::string strOut;
			char out[100] = {0};
			Global::utf8MultiToAnsiMulti((*it_array).asString(), strOut);
			msp_vr_grammar_trim(strOut.c_str(), out, strlen(strOut.c_str()));
			strcat(cmd, out);
		}
		else{
			strcat(cmd, "|");
			std::string strOut;
			char out[100] = { 0 };
			Global::utf8MultiToAnsiMulti((*it_array).asString(), strOut);
			msp_vr_grammar_trim(strOut.c_str(), out, strlen(strOut.c_str()));
			strcat(cmd, out);
		}
		//printf("%s,id=%d\n", (*it_array).asString().c_str(), id); 
		num++; 
	}
	return num;
}
#endif



}  // namespace commands

}  // namespace application_manager

