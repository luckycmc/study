

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
      public function list()
      {
            return [
                'name'=>'zpw',
                'schoole'=>'许昌学院'    
            ]
      }
}