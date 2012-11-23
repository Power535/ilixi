#ifndef ILIXI_PLATFORMMANAGER_H_
#define ILIXI_PLATFORMMANAGER_H_

#include <directfb.h>
#include <string>
#include <libxml/tree.h>
#include <map>
#include <types/Enums.h>
#include <types/Point.h>
#include <lib/Util.h>

namespace ilixi
{

/*!
 * PlatformManager helps with configuration and management of DirectFB interfaces.
 *
 * Settings are parsed from ilixi_config.xml file in DATADIR.
 */
class PlatformManager
{
    friend class AppBase;

public:
    /*!
     * Returns an instance of PlatformManager.
     */
    static PlatformManager&
    instance();

    /*!
     * Returns main DirectFB interface.
     */
    IDirectFB*
    getDFB() const;

    /*!
     * Returns a DFBLayer if logic layer name is matched, otherwise returns default ui layer.
     */
    IDirectFBDisplayLayer*
    getLayer(const std::string& name = "ui") const;

    /*!
     * Returns the layer surface only in exclusive mode, otherwise returns NULL.
     */
    IDirectFBSurface*
    getLayerSurface(const std::string& name = "ui") const;

    /*!
     * Returns application options.
     */
    AppOptions
    appOptions() const;

    /*!
     * Returns the flip mode used for given logic layer.
     */
    LayerFlipMode
    getLayerFlipMode(const std::string& name) const;

    /*!
     * Returns whether full screen updates are used for given logic layer.
     */
    bool
    useFSU(const std::string& name) const;

    /*!
     * Returns path to default palette definition file.
     */
    const std::string&
    getPalette() const;

    /*!
     * Returns path to default style definition file.
     */
    const std::string&
    getStyle() const;

    /*!
     * Render cursor using given position.
     */
    void
    renderCursor(const DFBPoint& point);

private:
    //! This property stores various options for application behaviour.
    AppOptions _options;

    //! DirectFB interface.
    IDirectFB* _dfb;

    IDirectFBDisplayLayer* _cursorLayer;
    IDirectFBSurface* _cursorTarget;
    IDirectFBSurface* _cursorImage;

    struct HardwareLayer
    {
        bool fsu;                       // FullScreenUpdates
        LayerFlipMode flipMode;              // Controls surface flip behaviour
        IDirectFBDisplayLayer* layer;
    };

    struct LogicLayer
    {
        unsigned int id;                // Id of layer
        IDirectFBSurface* surface;
    };

    typedef std::map<unsigned int, HardwareLayer> HardwareLayerMap;
    HardwareLayerMap _hwLayerMap;

    typedef std::map<std::string, LogicLayer> LogicLayerMap;
    LogicLayerMap _layerMap;

    std::string _style;
    std::string _palette;

    PlatformManager();

    PlatformManager(PlatformManager const& copy);

    PlatformManager&
    operator=(PlatformManager const& copy);

    /*!
     * Initialise DirectFB interfaces.
     *
     * This method is executed only once by main Application during its construction.
     */
    void
    initialize(int* argc, char*** argv, AppOptions opts);

    /*!
     * Releases DirectFB resources.
     */
    void
    release();

    bool
    parseConfig();

    void
    setHardwareLayers(xmlNodePtr node);

    void
    setLogicLayers(xmlNodePtr node);

#if ILIXI_DFB_VERSION >= VERSION_CODE(1,6,0)

    void
    setScreen(xmlNodePtr node);

    void
    setScreenTvStandard(DFBScreenEncoderTVStandards* cfg, xmlChar* standards);

    void
    setScreenOutConnectors(DFBScreenOutputConnectors* cfg, xmlChar* connectors);

    void
    setScreenScanMode(DFBScreenEncoderScanMode* cfg, xmlChar* scanmode);

    void
    setScreenResolution(DFBScreenOutputResolution* cfg, xmlChar* resolution);

    void
    setScreenFrequency(DFBScreenEncoderFrequency* cfg, xmlChar* freq);

#endif

    void
    setStylist(xmlNodePtr node);

    void
    setCursor(xmlNodePtr node);

    void
    setAppOption(AppOptions option);

    void
    unSetAppOption(AppOptions option);

    // what about service apps?
    // if there is a compositor, make video layer request via component.
};

} /* namespace ilixi */
#endif /* ILIXI_PLATFORMMANAGER_H_ */