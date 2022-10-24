

class Goods
{
       
      public function index()
      {    
            $returnData = [
                'name'=>'zpw',
                'age'=>'23',
                'sex'=>1,
            ];
            return json_encode($returnData);
      }
}