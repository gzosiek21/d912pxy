/*
MIT License

Copyright(c) 2018-2019 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#pragma once

//D3D9 API extenders =======================

#define D3DRS_ENABLE_D912PXY_API_HACKS (D3DRENDERSTATETYPE)220
#define D3DRS_D912PXY_ENQUEUE_PSO_COMPILE (D3DRENDERSTATETYPE)221
#define D3DRS_D912PXY_SETUP_PSO (D3DRENDERSTATETYPE)222
#define D3DRS_D912PXY_GPU_WRITE (D3DRENDERSTATETYPE)223
#define D3DRS_D912PXY_DRAW (D3DRENDERSTATETYPE)224
#define D3DRS_D912PXY_SAMPLER_ID (D3DRENDERSTATETYPE)225

#define D3DDECLMETHOD_PER_VERTEX_CONSTANT 8
#define D3DDECLMETHOD_PER_INSTANCE_CONSTANT 16
#define D3DUSAGE_D912PXY_FORCE_RT 0x0F000000L

#define D912PXY_ENCODE_GPU_WRITE_DSC(sz, offset) ((sz & 0xFFFF) | ((offset & 0xFFFF) << 16))

#define D912PXY_GPU_WRITE_OFFSET_TEXBIND 0
#define D912PXY_GPU_WRITE_OFFSET_SAMPLER 8 
#define D912PXY_GPU_WRITE_OFFSET_VS_VARS 16
#define D912PXY_GPU_WRITE_OFFSET_PS_VARS 16 + 256

//configuration switches =======================

#define UPLOAD_POOL_USE_AND_DISCARD 
//#define ENABLE_METRICS
//#define PERFORMANCE_GRAPH_WRITE_DX9
//#define PERFORMANCE_GRAPH_WRITE
//#define PER_BATCH_FLUSH_DEBUG 1
//#define TRACK_SHADER_BUGS_PROFILE

#ifdef PERFORMANCE_GRAPH_WRITE_DX9
	#define PERFORMANCE_GRAPH_WRITE
#endif

#ifdef _DEBUG
	//#define TRACK_SHADER_BUGS_PROFILE
	#define ENABLE_METRICS
	#define ENABLE_DEBUG_LOGGING
#endif

//inner max/structure defenitions =======================

#define d912pxy_shader_uid UINT64
#define d912pxy_shader_pair_hash_type UINT64

#define PXY_INNER_GPU_QUEUE_BUFFER_COUNT 2
#define PXY_INNER_MAX_SWAP_CHAINS 4
#define PXY_INNER_MAX_RENDER_TARGETS 8
#define PXY_INNER_MAX_TEXTURE_STAGES 32
#define PXY_INNER_MAX_SHADER_SAMPLERS 32
#define PXY_INNER_MAX_SHADER_SAMPLERS_SZ 1664
#define PXY_INNER_MAX_SHADER_CONSTS_IDX 256
#define PXY_INNER_MAX_SHADER_CONSTS 256*4
#define PXY_INNER_MAX_VBUF_STREAMS 10
#define PXY_INNER_MAX_IFRAME_CLEANUPS 1024*1024
#define PXY_INNER_MAX_CLEANUPS_PER_SYNC 128
#define PXY_INNER_MAX_IFRAME_BATCH_COUNT 1024*8
#define PXY_INNER_MAX_IFRAME_BATCH_REPLAY PXY_INNER_MAX_IFRAME_BATCH_COUNT*100
#define PXY_INNER_MAX_IFRAME_PSO_CACHES 4096
#define PXY_INNER_MAX_CACHE_NODES_SAMPLERS 4096
#define PXY_INNER_MAX_TEXSTATE_CACHE_NODES 0xFFFFF
#define PXY_INNER_MAX_PSO_CACHE_NODES 0xFFFF
#define PXY_INNER_MAX_DHEAP_CLEANUP_PER_SYNC 16
#define PXY_INNER_MAX_ASYNC_TEXLOADS 5120
#define PXY_INNER_MAX_ASYNC_BUFFERLOADS 5120

#define PXY_INNER_CLG_PRIO_FIRST 0 
#define PXY_INNER_CLG_PRIO_ASYNC_LOAD 1
#define PXY_INNER_CLG_PRIO_REPLAY 4
#define PXY_INNER_CLG_PRIO_LAST 100

#define PXY_INNER_MAX_DSC_HEAPS 4
#define PXY_INNER_HEAP_RTV 0
#define PXY_INNER_HEAP_DSV 1
#define PXY_INNER_HEAP_SRV 2
#define PXY_INNER_HEAP_CBV 2
#define PXY_INNER_HEAP_SPL 3

#define PXY_INNER_THREADID_TEX_LOADER 0
#define PXY_INNER_THREADID_BUF_LOADER 1
#define PXY_INNER_THREADID_RPL_THRD0 2
#define PXY_INNER_THREADID_RPL_THRD1 3
#define PXY_INNER_THREADID_RPL_THRD2 4
#define PXY_INNER_THREADID_RPL_THRD3 5
#define PXY_INNER_THREADID_MAX 6

#define PXY_INNER_REPLAY_THREADS_MAX 4

//shader profile defs =======================

#define PXY_INNER_SHDR_BUG_PCF_SAMPLER 0
#define PXY_INNER_SHDR_BUG_ALPHA_TEST 1
#define PXY_INNER_SHDR_BUG_SRGB_READ 2
#define PXY_INNER_SHDR_BUG_SRGB_WRITE 3
#define PXY_INNER_SHDR_BUG_CLIPPLANE0 4
#define PXY_INNER_SHDR_BUG_RESERVED0 5
#define PXY_INNER_SHDR_BUG_RESERVED1 6
#define PXY_INNER_SHDR_BUG_RESERVED2 7
#define PXY_INNER_SHDR_BUG_RESERVED3 8
#define PXY_INNER_SHDR_BUG_COUNT 9
#define PXY_INNER_SHDR_BUG_FILE_SIZE PXY_INNER_SHDR_BUG_COUNT * 4

//metrics macros =======================

#ifdef ENABLE_METRICS
	#define FRAME_METRIC_CLEANUPS(a) d912pxy_s(metrics)->TrackCleanupCount(a);
	#define FRAME_METRIC_DHEAP(a,b) d912pxy_s(metrics)->TrackDHeapSlots(a,b);
	#define FRAME_METRIC_EXEC(a) d912pxy_s(metrics)->TrackIFrameTime(a, PXY_METRICS_IFRAME_EXEC);
	#define FRAME_METRIC_SYNC(a) d912pxy_s(metrics)->TrackIFrameTime(a, PXY_METRICS_IFRAME_SYNC);
	#define FRAME_METRIC_SYNC_WAKE(a) d912pxy_s(metrics)->TrackIFrameTime(a, PXY_METRICS_IFRAME_SYNC_WAKE);
	#define FRAME_METRIC_THREAD(a,b) d912pxy_s(metrics)->TrackIFrameTime(a, PXY_METRICS_IFRAME_THREAD_TEX+b);
	#define FRAME_METRIC_PRESENT(a) d912pxy_s(metrics)->TrackIFrameTime(a, PXY_METRICS_IFRAME_PREP);
	#define API_OVERHEAD_TRACK_START(a) d912pxy_s(metrics)->TrackAPIOverheadStart(API_OVERHEAD_TRACK_LOCAL_ID_DEFINE);
	#define API_OVERHEAD_TRACK_END(a) d912pxy_s(metrics)->TrackAPIOverheadEnd(API_OVERHEAD_TRACK_LOCAL_ID_DEFINE);	
#else 
	#define FRAME_METRIC_CLEANUPS(a)
	#define FRAME_METRIC_DHEAP(a,b)
	#define FRAME_METRIC_EXEC(a) 
	#define FRAME_METRIC_SYNC(a)
	#define FRAME_METRIC_SYNC_WAKE(a)
	#define FRAME_METRIC_THREAD(a,b)
	#define FRAME_METRIC_PRESENT(a) 
	#define API_OVERHEAD_TRACK_START(a)
	#define API_OVERHEAD_TRACK_END(a)
#endif

//logging macro =======================

#ifdef DISABLE_P7LIB
	#define TM(i_pStr)         L##i_pStr
#endif

#define LOG_INFO_DTDM(fmt, ...) (d912pxy_s(log)->_PXY_LOG_INFO(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
#define LOG_WARN_DTDM(fmt, ...) (d912pxy_s(log)->_PXY_LOG_WARNING(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
#define LOG_ERR_DTDM(fmt, ...) (d912pxy_s(log)->_PXY_LOG_ERROR(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
#define LOG_INFO_DTDM2(code, fmt, ...) code; LOG_INFO_DTDM(fmt, __VA_ARGS__)

#ifdef ENABLE_DEBUG_LOGGING
	#ifdef _DEBUG
		#define LOG_DBG_DTDM(fmt, ...) ;//(d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
		#define LOG_DBG_DTDM2(fmt, ...) ;//(d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
		#define LOG_DBG_DTDM3(fmt, ...) (d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
	#else 
		#define LOG_DBG_DTDM(fmt, ...) (d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
		#define LOG_DBG_DTDM2(fmt, ...) (d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
		#define LOG_DBG_DTDM3(fmt, ...) (d912pxy_s(log)->_PXY_LOG_DEBUG(LGC_DEFAULT, TM(fmt), __VA_ARGS__))
	#endif
#else
	#define LOG_DBG_DTDM(fmt, ...) ;
	#define LOG_DBG_DTDM2(fmt, ...) ;
	#define LOG_DBG_DTDM3(fmt, ...) ;
#endif

#define LOG_ERR_THROW(hr) LOG_ERR_THROW2(hr, hr)
#define LOG_ERR_THROW2(hr, hr2) ThrowErrorDbg(hr, #hr2 )

//paths to files =======================

#define d912pxy_cs_hlsl_dir L"./d912pxy/shaders/cs"
#define d912pxy_cs_cso_dir "cs/cso"
#define d912pxy_shader_db_hlsl_dir L"./d912pxy/shaders/hlsl"
#define d912pxy_shader_db_hlsl_custom_dir L"./d912pxy/shaders/hlsl/custom"
#define d912pxy_shader_db_cso_dir "shaders/cso"
#define d912pxy_shader_db_bugs_dir "shaders/bugs"
#define PXY_CRASH_LOG_FILE_PATH "d912pxy/crash"
#define PXY_LOG_FILE_NAME "d912pxy/log.txt"

//forward class defenitions =======================

class d912pxy_basetexture;
class d912pxy_vtexture;
class d912pxy_texture;
class d912pxy_ctexture;
class d912pxy_surface;
class d912pxy_device;
class d912pxy_dheap;
class d912pxy_vdecl;
class d912pxy_gpu_cmd_list;
class d912pxy_replay_base;
class d912pxy_batch;
class d912pxy_shader_db;
class d912pxy_shader_pair;
class d912pxy_iframe;
class d912pxy_upload_pool;
class d912pxy_upload_item;
class d912pxy_vstream_pool;
class d912pxy_vstream;
class d912pxy_surface_pool;
class d912pxy_cleanup_thread;
class d912pxy_gpu_que;
class d912pxy_gpu_cmd_list;
class d912pxy_texture_state;
class d912pxy_surface_layer;
class d912pxy_texture_loader;
class d912pxy_buffer_loader;
class d912pxy_pso_cache;
class d912pxy_batch;
class d912pxy_pso_cache_item;
class d912pxy_vfs;
class d912pxy_metrics;
class d912pxy_config;
class d912pxy_log;
struct d912pxy_trimmed_dx12_pso;


typedef struct d912pxy_device_streamsrc {
	d912pxy_vstream* buffer;
	UINT offset;
	UINT stride;
	UINT divider;
} d912pxy_device_streamsrc;

typedef enum d912pxy_gpu_cmd_list_group {
	CLG_TOP = 0,
	CLG_TEX = 1,
	CLG_BUF = 2,
	CLG_RP1 = 3,
	CLG_RP2 = 4,
	CLG_RP3 = 5,
	CLG_RP4 = 6,
	CLG_SEQ = 7
} d912pxy_gpu_cmd_list_group;

#define PXY_INNER_MAX_GPU_CMD_LIST_GROUPS 8

//global singletons =======================

class d912pxy_global_objects {
public:
	d912pxy_global_objects() {  };
	~d912pxy_global_objects() {  };

	static d912pxy_shader_db* sdb;
	static d912pxy_vstream_pool* pool_vstream;
	static d912pxy_surface_pool* pool_surface;
	static d912pxy_upload_pool* pool_upload;
	static d912pxy_cleanup_thread* thread_cleanup;
	static d912pxy_iframe* iframe;
	static d912pxy_gpu_que* GPUque;
	static d912pxy_gpu_cmd_list* GPUcl;
	static ID3D12Device* DXDev;
	static d912pxy_replay_base* CMDReplay;
	static d912pxy_texture_state* textureState;
	static d912pxy_pso_cache* psoCache;
	static d912pxy_texture_loader* texloadThread;
	static d912pxy_buffer_loader* bufloadThread;
	static d912pxy_batch* batch;	
	static d912pxy_vfs* vfs;
	static d912pxy_device* dev;
	static d912pxy_metrics* metrics;
	static d912pxy_config* config;
	static d912pxy_log* log;
};

#define d912pxy_s(a) d912pxy_global_objects::a
#define d912pxy_s_dcl(a,t) t* d912pxy_global_objects::a = 0;
