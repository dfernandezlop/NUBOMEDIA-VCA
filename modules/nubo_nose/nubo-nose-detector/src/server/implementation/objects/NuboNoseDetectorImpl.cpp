
/* Autogenerated with kurento-module-creator */

#include <gst/gst.h>
#include <vector>
#include <NoseInfo.hpp> 
#include "MediaPipeline.hpp"
#include "MediaPipelineImpl.hpp"
#include <NuboNoseDetectorImplFactory.hpp>
#include "NuboNoseDetectorImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include <SignalHandler.hpp>

#define GST_CAT_DEFAULT kurento_nubo_nose_detector_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoNuboNoseDetectorImpl"

#define VIEW_NOSES "view-noses"
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
    namespace nubonosedetector
    {

    void  NuboNoseDetectorImpl::split_message (std::string fi, std::string delimiter, 
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

 
void NuboNoseDetectorImpl::onNose (gchar *message)
{

  /*the String received will be like this ( the ; is the seperation among noses): 
    x:int,y:int,width:int,height:int;x:int,y:int,width:int,height:int;*/
  std::string del1 = ";";
  std::string del2 = ",";
  std::string del3 = ":";
  std::vector<std::string> *noses = new std::vector<std::string>;
  std::vector<std::string> *fields = new std::vector<std::string>;
  std::vector<std::string> *all = new std::vector<std::string>;
  std::vector<std::shared_ptr<NoseInfo>> test;
  int x,y,height,width;
  std::string t; 
  
  bool register_completed=true;
  int i=0;

  try {
    
    split_message(message,del1,noses);
    for( i=0; (int)(noses->size())>i; i++) 
      split_message(noses->at(i),del2,fields);
    
    for( i=0; (int)(fields->size())>i; i++)
      split_message(fields->at(i),del3,all);
    
    NoseInfo *fi;
    
    for(int i=0; (int)(all->size())>i; i=i+2)
      {
	
	if (register_completed)
	  {
	    t="nose";
	    x=0;y=0;width=0;height=0;
	    register_completed=false;
	  }
	if (0==all->at(i).compare("x")) 	
	  {
	    if ((i+1)< (int)(all->size()))	      
	      x=std::stoi(all->at(i+1));
	  }
	else if (0==all->at(i).compare("y")) 
	  {
	    if ((i+1)< (int)(all->size()))
	      y=std::stoi(all->at(i+1));
	  }
	
	else if (0==all->at(i).compare("width"))     
	  {
	    if ((i+1)< (int)(all->size()))
	      width=std::stoi(all->at(i+1));
	  }
	else if (0==all->at(i).compare("height"))     
	  {
	    if ((i+1)< (int)(all->size()))
	      {
		height=std::stoi(all->at(i+1));
		std::shared_ptr <NoseInfo> cand ( new NoseInfo(t,x,y,width,height));
		test.push_back(cand);
		register_completed=true;
	      }
	  }
      }
   
    if (test.size()>0)
      {
	OnNose event (shared_from_this(), OnNose::getName(), test);
	signalOnNose (event);
      }
  } catch (std::bad_weak_ptr &e) {
  }
}

void NuboNoseDetectorImpl::postConstructor ()
{
    handlerOnNoseEvent = register_signal_handler (G_OBJECT (nubo_nose),
		        "nose-event",
			 std::function <void (GstElement *, gchar *) >
			 (std::bind (&NuboNoseDetectorImpl::onNose, this,
                         std::placeholders::_2) ),
                          std::dynamic_pointer_cast<NuboNoseDetectorImpl>
		          (shared_from_this() ) );
}
      NuboNoseDetectorImpl::NuboNoseDetectorImpl (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline)  : FilterImpl (config, std::dynamic_pointer_cast<MediaPipelineImpl> (mediaPipeline)) 
      {
 
	g_object_set (element, "filter-factory", "nubonosedetector", NULL);
  
	g_object_get (G_OBJECT (element), "filter", &nubo_nose, NULL);

	if (NULL == nubo_nose) {
	  throw KurentoException (MEDIA_OBJECT_NOT_AVAILABLE,
				  "Media Object not available");
	}
        handlerOnNoseEvent = 0;
	g_object_unref(nubo_nose); 
      }

      void NuboNoseDetectorImpl::showNoses (int viewNoses)
      {
	g_object_set(G_OBJECT (nubo_nose), VIEW_NOSES, viewNoses, NULL);
      }


      void NuboNoseDetectorImpl::detectByEvent(int event)
      {
  
	g_object_set(G_OBJECT (nubo_nose), DETECT_BY_EVENT, event, NULL);
  
      }
  
      void NuboNoseDetectorImpl::sendMetaData(int metaData)
      {
	g_object_set(G_OBJECT (nubo_nose),SEND_META_DATA , metaData, NULL);
      }

      void NuboNoseDetectorImpl::multiScaleFactor(int scaleFactor)
      {
	
	g_object_set(G_OBJECT (nubo_nose),FACTOR_SCALE , scaleFactor, NULL);
      }

      void NuboNoseDetectorImpl::processXevery4Frames(int xper4)
      {
	g_object_set(G_OBJECT (nubo_nose),X_EVERY_4_FRAMES , xper4, NULL);
      }

      void NuboNoseDetectorImpl::widthToProcess(int width)
      {
	g_object_set(G_OBJECT (nubo_nose),WIDTH_TO_PROCESS , width, NULL);
      }

      void NuboNoseDetectorImpl::activateServerEvents (int activate,int ms)
      {
        g_object_set (G_OBJECT (nubo_nose), ACTIVATE_EVENTS , activate, NULL);
        g_object_set (G_OBJECT (nubo_nose), EVENTS_MS , ms , NULL);
      }

      void NuboNoseDetectorImpl::unsetOverlayedImage ()
      {
        GstStructure *imageSt;
        imageSt = gst_structure_new ("image",
                                     "offsetXPercent", G_TYPE_DOUBLE, 0.0,
                                     "offsetYPercent", G_TYPE_DOUBLE, 0.0,
                                     "widthPercent", G_TYPE_DOUBLE, 0.0,
                                     "heightPercent", G_TYPE_DOUBLE, 0.0,
                                     "url", G_TYPE_STRING, NULL,
                                     NULL);
        g_object_set (G_OBJECT (nubo_nose), "image-to-overlay", imageSt, NULL);
        gst_structure_free (imageSt);
      }

      void NuboNoseDetectorImpl::setOverlayedImage (const std::string &uri, float offsetXPercent, float offsetYPercent, float widthPercent, float heightPercent)
      {
        GstStructure *imageSt;
        imageSt = gst_structure_new ("image",
                                     "offsetXPercent", G_TYPE_DOUBLE, double (offsetXPercent),
                                     "offsetYPercent", G_TYPE_DOUBLE, double (offsetYPercent),
                                     "widthPercent", G_TYPE_DOUBLE, double (widthPercent),
                                     "heightPercent", G_TYPE_DOUBLE, double (heightPercent),
                                     "url", G_TYPE_STRING, uri.c_str(),
                                     NULL);
        g_object_set (G_OBJECT (nubo_nose), "image-to-overlay", imageSt, NULL);
        gst_structure_free (imageSt);
      }

      MediaObjectImpl *
      NuboNoseDetectorImplFactory::createObject (const boost::property_tree::ptree &config, std::shared_ptr<MediaPipeline> mediaPipeline) const
      {
	return new NuboNoseDetectorImpl (config, mediaPipeline);
      }

      NuboNoseDetectorImpl::StaticConstructor NuboNoseDetectorImpl::staticConstructor;

      NuboNoseDetectorImpl::StaticConstructor::StaticConstructor()
      {
	GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
				 GST_DEFAULT_NAME);
      }

      NuboNoseDetectorImpl::~NuboNoseDetectorImpl()
      {
	if ( handlerOnNoseEvent > 0) {
	  unregister_signal_handler (nubo_nose, handlerOnNoseEvent );
	}
      }
    } /* nubonosedetector */
  } /* module */
} /* kurento */
