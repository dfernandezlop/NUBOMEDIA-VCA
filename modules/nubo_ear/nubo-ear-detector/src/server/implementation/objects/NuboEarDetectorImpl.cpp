/* Autogenerated with kurento-module-creator */

#include <gst/gst.h>
#include <vector>
#include <EarInfo.hpp> 
#include "MediaPipeline.hpp"
#include "MediaPipelineImpl.hpp"
#include <NuboEarDetectorImplFactory.hpp>
#include "NuboEarDetectorImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include <SignalHandler.hpp>

#define GST_CAT_DEFAULT kurento_nubo_ear_detector_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoNuboEarDetectorImpl"

#define VIEW_EARS "view-ears"
#define DETECT_BY_EVENT "detect-event"
#define SEND_META_DATA "send-meta-data"
#define FACTOR_SCALE "multi-scale-factor"
#define X_EVERY_4_FRAMES "process-x-every-4-frames"
#define WIDTH_TO_PROCESS "width-to-process"
#define ACTIVATE_EVENTS "activate-events"
#define EVENTS_MS "events-ms"

namespace kurento
{
  namespace module
  {
    namespace nuboeardetector
    {
      
      void  NuboEarDetectorImpl::split_message (std::string fi, std::string delimiter, 
					     std::vector<std::string> *v)
      {
	size_t pos = 0;
	std::string token;
	
	while ((pos = fi.find(delimiter)) != std::string::npos) {
	  token = fi.substr(0, pos);  
	  v->push_back(token);
	  fi.erase(0, pos + delimiter.length());
	}
	
	v->push_back(fi);
	
      }

      
      void NuboEarDetectorImpl::onEar (gchar *message)
      {

	/*the String received will be like this ( the ; is the seperation among ears): 
	  x:int,y:int,width:int,height:int;x:int,y:int,width:int,height:int;*/
	std::string del1 = ";";
	std::string del2 = ",";
	std::string del3 = ":";
	std::vector<std::string> *ears = new std::vector<std::string>;
	std::vector<std::string> *fields = new std::vector<std::string>;
	std::vector<std::string> *all = new std::vector<std::string>;
	std::vector<EarInfo> *f_info = new std::vector<EarInfo>;
  
	bool register_completed=true;
	int i=0;
  
	try {
    
	  split_message(message,del1,ears);
	  for( i=0; (int)(ears->size())>i; i++) 
	    split_message(ears->at(i),del2,fields);
    
	  for( i=0; (int)(fields->size())>i; i++)
	    split_message(fields->at(i),del3,all);
    
	  EarInfo *mi;
    
	  for(int i=0; (int)(all->size())>i; i=i+2)
	    {
	
	      if (register_completed)
		{
		  mi= new EarInfo("ear",-1,-1,-1,-1);
		  register_completed=false;
		}
	      if (0==all->at(i).compare("x")) 	
		{
		  if ((i+1)< (int)(all->size()))	      
		    mi->setX(std::stoi(all->at(i+1)));
		}
	      else if (0==all->at(i).compare("y")) 
		{
		  if ((i+1)< (int)(all->size()))
		    mi->setY(std::stoi(all->at(i+1)));
		}
	
	      else if (0==all->at(i).compare("width"))     
		{
		  if ((i+1)< (int)(all->size()))
		    mi->setWidth(std::stoi(all->at(i+1)));
		}
	      else if (0==all->at(i).compare("height"))     
		{
		  if ((i+1)< (int)(all->size()))
		    {
		      mi->setHeight(std::stoi(all->at(i+1)));
		      register_completed=true;
		      f_info->push_back(*mi);
		    }
		}
	    }

	  std::vector<std::shared_ptr<EarInfo>> test;
    
	  for (i=0;i<(int)(f_info->size());i++)
	    {
	      std::shared_ptr <EarInfo> f1 (&(f_info->at(i)));
	      test.push_back(f1);
	    }
   
	  OnEar event (shared_from_this(), OnEar::getName(), test);
	  signalOnEar (event);
	} catch (std::bad_weak_ptr &e) {
	}
      }
      
      void NuboEarDetectorImpl::postConstructor ()
      {
	handlerOnEarEvent = register_signal_handler (G_OBJECT (nubo_ear),
			      "ear-event",
			       std::function <void (GstElement *, gchar *) >
			       (std::bind (&NuboEarDetectorImpl::onEar, this,
			        std::placeholders::_2) ),
			       std::dynamic_pointer_cast<NuboEarDetectorImpl>
			      (shared_from_this() ) );
      }

      NuboEarDetectorImpl::NuboEarDetectorImpl (const boost::property_tree::ptree &config,
						std::shared_ptr<MediaPipeline> mediaPipeline):FilterImpl 
						(config,
						 std::dynamic_pointer_cast<MediaPipelineImpl> (mediaPipeline))
      {
	g_object_set (element, "filter-factory", "nuboeardetector", NULL);
  
	g_object_get (G_OBJECT (element), "filter", &nubo_ear, NULL);
  
	if (NULL == nubo_ear) {
	  throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
				  "Media Object not available");
	}
  
	g_object_unref(nubo_ear); 
      }

      void NuboEarDetectorImpl::showEars (int viewEars)
      {
	g_object_set(G_OBJECT (nubo_ear), VIEW_EARS, viewEars, NULL);
      }


      void NuboEarDetectorImpl::detectByEvent(int event)
      {
  
	g_object_set(G_OBJECT (nubo_ear), DETECT_BY_EVENT, event, NULL);
  
      }
  
      void NuboEarDetectorImpl::sendMetaData(int metaData)
      {
	g_object_set(G_OBJECT (nubo_ear),SEND_META_DATA , metaData, NULL);
      }

      
      void NuboEarDetectorImpl::multiScaleFactor(int scaleFactor)
      {
	g_object_set(G_OBJECT (nubo_ear),FACTOR_SCALE , scaleFactor, NULL);
      }
      
      void NuboEarDetectorImpl::processXevery4Frames(int xper4)
      {
	g_object_set(G_OBJECT (nubo_ear),X_EVERY_4_FRAMES , xper4, NULL);
      }
      
      void NuboEarDetectorImpl::widthToProcess(int width)
      {
 	g_object_set(G_OBJECT (nubo_ear),WIDTH_TO_PROCESS , width, NULL);
      }

      void NuboEarDetectorImpl::activateServerEvents (int activate,int ms)
      {
	g_object_set (G_OBJECT (nubo_ear), ACTIVATE_EVENTS , activate, NULL);
	g_object_set (G_OBJECT (nubo_ear), EVENTS_MS , ms , NULL);
      }


      MediaObjectImpl *
      NuboEarDetectorImplFactory::createObject (const boost::property_tree::ptree &config,
						std::shared_ptr<MediaPipeline> mediaPipeline) const
      {
	return new NuboEarDetectorImpl (config, mediaPipeline);
      }


      NuboEarDetectorImpl::StaticConstructor NuboEarDetectorImpl::staticConstructor;



      NuboEarDetectorImpl::StaticConstructor::StaticConstructor()
      {
	GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
				 GST_DEFAULT_NAME);
      }

      NuboEarDetectorImpl::~NuboEarDetectorImpl()
      {
	if ( handlerOnEarEvent > 0) {
	  unregister_signal_handler (nubo_ear, handlerOnEarEvent );
	}
      }

    } /* nuboeardetector */
  } /* module */
} /* kurento */
