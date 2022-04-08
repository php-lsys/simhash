<?php
/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
namespace LsExt;
if (!class_exists(SimHash::class)){
class SimHash
{
	protected $fingerprint;
	
	public function __construct($fingerprint)
	{
		$this->fingerprint = $fingerprint;
	}
	
	public function compareWith(SimHash $otherHash)
	{
		$differences = substr_count(decbin($this->getDec() ^ $otherHash->getDec()), '1');
		$fpLength = strlen((string) decbin($this->getDec())) * 2;
		
		return 1 - ($differences / $fpLength);
	}
	public function hammingDistance(SimHash $otherHash)
    {
        return substr_count(decbin($this->getDec() ^ $otherHash->getDec()), '1');
    }
	
	public function __toString()
	{
		return $this->getDec();
	}
	
	public function getBin()
	{
		return decbin($this->getDec());
	}
	
	public function getHex()
	{
		return dechex($this->getDec());
	}
	
	public function getDec()
	{
		return $this->fingerprint;
	}
}
}