#include <jni.h>

#include "config_profile/android_profile.h"
#include "appMain/life_cycle_impl.h"
#include "utils/lock.h"
#include "utils/conditional_variable.h"

#ifdef ENABLE_LOG
#include "utils/logger/androidlogger.h"
#include "utils/logger/logger_impl.h"
#endif  // ENABLE_LOG

SDL_CREATE_LOCAL_LOG_VARIABLE("Main")

void StartSDL(JNIEnv* env, jobject);
void StopSDL(JNIEnv* env, jobject);

static JNINativeMethod s_methods[] = {
   {"StartSDL", "()V", (void*)StartSDL},
   {"StopSDL", "()V", (void*)StopSDL}
};

// To call Java methods when running native code inside an Android activity,
// a reference is needed to the JavaVM
static JavaVM *gJavaVM;

sync_primitives::Lock wait_lock_;
sync_primitives::ConditionalVariable wait_var_;

std::string JNI_GetMainActivityStringProperty(const char* property) {
    JNIEnv *env = NULL;
    gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    jclass cls = env->FindClass("org/luxoft/sdl_core/MainActivity");
    jfieldID fieldID = env->GetStaticFieldID(cls, property, "Ljava/lang/String;");
    auto fieldValue = static_cast<jstring>(env->GetStaticObjectField(cls, fieldID));

    const char *path_chars = env->GetStringUTFChars(fieldValue, NULL);
    return std::string(path_chars);
}

jint JNI_OnLoad(JavaVM* vm, void*) {
    gJavaVM = vm;
    JNIEnv *env = NULL;
    gJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    jclass cls = env->FindClass("org/luxoft/sdl_core/MainActivity");
    auto globalClass = reinterpret_cast<jclass>(env->NewGlobalRef(cls));

    int len = sizeof(s_methods) / sizeof(s_methods[0]);

    env->RegisterNatives(globalClass, s_methods, len);

    return JNI_VERSION_1_6;
}

void StartSDL(JNIEnv* env, jobject)
{
#ifdef ENABLE_LOG
    auto logger_impl =
            std::unique_ptr<logger::LoggerImpl>(new logger::LoggerImpl());
    logger::Logger::instance(logger_impl.get());
#endif  // ENABLE_LOG
    const std::string internal_storage = JNI_GetMainActivityStringProperty("sdl_cache_folder_path");
    const std::string external_storage = JNI_GetMainActivityStringProperty("sdl_external_dir_folder_path");

	profile::AndroidProfile profile_instance(internal_storage, external_storage);
	std::unique_ptr<main_namespace::LifeCycle> life_cycle(
      new main_namespace::LifeCycleImpl(profile_instance));


    std::string ini_name = "androidSmartDeviceLink.ini";
	if (!internal_storage.empty()) {
	    ini_name = internal_storage + "/files/" + ini_name;
	}

    profile_instance.set_config_file_name(ini_name);

#ifdef ENABLE_LOG
  if (profile_instance.logs_enabled()) {
    // Logger initialization
    // Redefine for each paticular logger implementation
    auto logger = std::unique_ptr<logger::AndroidLogger>(
        new logger::AndroidLogger());
    logger_impl->Init(std::move(logger));
  }
#endif

    SDL_LOG_INFO("Application started!");
    SDL_LOG_INFO("SDL version: " << profile_instance.sdl_version());

    // Check if no error values were read from config file
    if (profile_instance.ErrorOccured()) {
        SDL_LOG_FATAL(profile_instance.ErrorDescription());
        SDL_DEINIT_LOGGER();
        exit(EXIT_FAILURE);
    }

    if (!life_cycle->StartComponents()) {
        SDL_LOG_FATAL("Failed to start components");
        life_cycle->StopComponents();
        SDL_DEINIT_LOGGER();
        return;
    }

    SDL_LOG_INFO("Components Started");

    if (!life_cycle->InitMessageSystem()) {
        SDL_LOG_FATAL("Failed to init message system");
        life_cycle->StopComponents();
        SDL_DEINIT_LOGGER();
        return;
    }

    {
        // TODO: Replace with lifecycle Run() once signal handling is fixed
        sync_primitives::AutoLock auto_lock(wait_lock_);
        wait_var_.Wait(auto_lock);
    }

    SDL_LOG_INFO("Stop SDL due to caught signal");

    life_cycle->StopComponents();
    SDL_LOG_INFO("Application has been stopped successfully");

    SDL_LOG_TRACE("StartSDL: exit");

    SDL_DEINIT_LOGGER();
}

void StopSDL(JNIEnv*, jobject) {
    SDL_LOG_INFO("Stop from main activity requested");
    {
        // TODO: Replace with sigkill(SIGINT) once signal handling is fixed
        sync_primitives::AutoLock auto_lock(wait_lock_);
        wait_var_.NotifyOne();
    }
}