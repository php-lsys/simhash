<?php
/**
 * lsys core
 * @author     Lonely <shan.liu@msn.com>
 * @copyright  (c) 2017 Lonely <shan.liu@msn.com>
 * @license    http://www.apache.org/licenses/LICENSE-2.0
 */
namespace LsExt;
if (!class_exists(SimHashFactory::class)){
class SimHashFactory
{
	protected $hashSize;
	protected $weight;
	public function __construct($hash_size = 32)
	{
		$this->hashSize = $hash_size;
	}
	public function setWeight(array $weight){
	    $this->weight=$weight;
	    return $this;
	}
    protected function  _tokenizer ($words)
    {
        $tokens = array();
        $hash_method = function ($str)
        {
            $str_hex = md5($str);
            $str_bin = '';

            for ($i = 0; $i < strlen($str_hex); $i++)
            {
                $str_bin .= sprintf('%04s', decbin(hexdec($str_hex[$i])));
            }
            return (!empty($str_bin)) ? $str_bin : false;
        };
        foreach (array_count_values($words) as $key => $weight)
        {
            $tokens[$key]['weight'] = $weight;
            $tokens[$key]['hash'] = $hash_method($key);
        }
        return $tokens;
    }
	protected function _vectorize(array $tokens)
	{
		$vector = array_fill(0, $this->hashSize, 0);
		
		foreach($tokens as $key => $value)
		{
			for ($i = 0; $i < $this->hashSize; $i++)
			{
			    $w=isset($this->weight[$key])?$this->weight[$key]:1;
				if ($value['hash'][$i] == 1)
					$vector[$i] = intval($vector[$i]) + intval($value['weight'])*$w;
				else
					$vector[$i] = intval($vector[$i]) - intval($value['weight'])*$w;
			}
		}
		
		return $vector;
	}
	/**
	 * @param array $words
	 * @return number
	 */
	public function fingerprint(array $words)
	{
		$vector=$this->_vectorize($this->_tokenizer($words));
		$fingerprint = str_pad('', $this->hashSize, '0');
		for ($i = 0; $i < $this->hashSize; $i++)
		{
			if ($vector[$i] >= 0) $fingerprint[$i] = '1';
		}
		return bindec($fingerprint);
	}
}
}