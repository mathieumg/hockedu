<?php
/**
 * Cache management for the Hockedu System.
 * 
 * Manages the caching in Hockedu.
 * @author Mathieu M-Gosselin <mathieumg@gmail.com>
 * @since 17/02/2013
 * @package Hockedu
 */
 
 
 /**
 * CacheStore
 *
 * Manages the loading and saving of cache elements.
 * @package CacheStore
 */
class CacheStore
{
    /**
     * Cache data.
     * @access private
     * @var array
     */
    private $data;
    
    /**
     * Whether the current CacheStore exists or not.
     * @access private
     * @var boolean
     */
    private $exists;
    
    /**
     * Name of the CacheStore.
     * @access private
     * @var string
     */
    private $name;
    
    /**
     * Path where the cache files are stored.
     * @access private
     * @var string
     */
    private $path;
    
    /**
     * Type of cache (file/memcache/etc.).
     * @access private
     * @var integer
     */
    private $type;
    
    /**
     * Constructor.
     * @param string Unique name of the CacheStore.
     */
    public function __construct( $name )
    {
        // Some default values for attributes.
        $this->exists = false;
        $this->path = '/var/www/hockedu.com/cache/';
        $this->data = null;
        $this->name = $name;
        
        // Is that cache stored somewhere?
        if( file_exists( $this->path . $this->name . '.inc.php' ) )
        {
            $this->exists = true;
        }
    }
    
    /**
     * Returns the data contained in the cache.
     * @access public
     * @return mixed Returns a variable with the cache data in it.
     */
    public function getCacheData()
    {
        // Try to load the cache if it's empty.
        if( ( $this->data === null ) && ( $this->exists() === true ) )
        {
            require_once( $this->path . $this->name . '.inc.php' );
            
            $this->data = $cacheContent;
        }
        
        return $this->data;
    }
    
    /**
     * Sets the data in the cache.
     * @access public
     * @param array Cache data to import.
     */
    public function setCacheData( $data )
    {
        $this->data = $data;
    }
    
    /**
     * Indicates whether the current CacheStore exists or not (i.e. whether it is used for reading
     * or for writing).
     * @access public
     * @return boolean True if that CacheStore already exists in some sort of storage.
     */
    public function exists()
    {
        return $this->exists;
    }
    
    /**
     * Saves the cache data.
     * @access public
     * @param string media.
     */
    public function save( $media = 'file' )
    {
        if( $media === 'file' )
        {
            // Write to the cache to a file.
            $fileHandle = fopen( $this->path . $this->name . '.inc.php', 'w' );
            fwrite( $fileHandle, "<?php\n" . '$cacheContent = ' . var_export( $this->data, true ) . ";\n" );
            fclose( $fileHandle );
        }
    }
}
