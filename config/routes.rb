ActionController::Routing::Routes.draw do |map|
  map.search 'home/search', :controller => 'rooms', :action => 'search'
  map.search 'profiles/search', :controller => 'rooms', :action => 'search'
  map.search 'sessions/search', :controller => 'rooms', :action => 'search'
  map.search 'rooms/search', :controller => 'rooms', :action => 'search'
  map.search 'room_profiles/search', :controller => 'rooms', :action => 'search'
  map.search 'hosts/search', :controller => 'rooms', :action => 'search'

  map.all '/rooms/all', :controller => 'rooms', :action => 'all'
  map.show_private '/rooms/show_private', :controller => 'rooms', :action => 'show_private'
  map.requested '/rooms/requested', :controller => 'rooms', :action => 'requested'
  map.manage_transaction '/transactions/manage_transaction', :controller => 'transactions', :action => 'manage_transaction'
  
  map.logout '/logout', :controller => 'sessions', :action => 'destroy'
  map.login '/login', :controller => 'sessions', :action => 'new'
  map.register '/register', :controller => 'users', :action => 'create'
  map.signup '/signup', :controller => 'users', :action => 'new'
  map.forgot_password '/forgot_password', :controller => 'users', :action => 'forgot_password'
  map.reset_password '/reset_password/:id', :controller => 'users', :action => 'reset_password' 
  map.change_password '/change_password', :controller => 'users', :action => 'change_password'
  map.suspend '/profiles/suspend', :controller => 'users', :action => 'suspend'
  map.create '/marker/create_marker', :controller=>'marker', :action=>'create'
  map.list '/marker/list', :controller=>'marker', :action=>'list'
   
  map.activate '/activate/:activation_code', 
             :controller => 'users', 
             :action => 'activate', 
             :activation_code => nil
  
  map.resources :users, :member => { :suspend   => :put,
                                   :unsuspend => :put,
                                   :purge     => :delete }
  
  
  
  map.resource :session
  
  map.root :controller => 'home'
  map.home ':page', :controller => 'home', :action => 'show', :page => /home|about|contact|privacy/
  
  map.resources :room_reviews

  map.resources :rooms

  map.resources :favorites

  map.resources :user_reviews

  map.resources :profiles

  map.resources :users

  map.resources :modifications

  map.resources :transactions

  map.resources :renters

  map.resources :hosts 
  
  map.resources :room_profiles

  map.resources :marker
  # The priority is based upon order of creation: first created -> highest priority.

  # Sample of regular route:
  #   map.connect 'products/:id', :controller => 'catalog', :action => 'view'
  # Keep in mind you can assign values other than :controller and :action

  # Sample of named route:
  #   map.purchase 'products/:id/purchase', :controller => 'catalog', :action => 'purchase'
  # This route can be invoked with purchase_url(:id => product.id)

  # Sample resource route (maps HTTP verbs to controller actions automatically):
  #   map.resources :products

  # Sample resource route with options:
  #   map.resources :products, :member => { :short => :get, :toggle => :post }, :collection => { :sold => :get }

  # Sample resource route with sub-resources:
  #   map.resources :products, :has_many => [ :comments, :sales ], :has_one => :seller
  
  # Sample resource route with more complex sub-resources
  #   map.resources :products do |products|
  #     products.resources :comments
  #     products.resources :sales, :collection => { :recent => :get }
  #   end

  # Sample resource route within a namespace:
  #   map.namespace :admin do |admin|
  #     # Directs /admin/products/* to Admin::ProductsController (app/controllers/admin/products_controller.rb)
  #     admin.resources :products
  #   end

  # You can have the root of your site routed with map.root -- just remember to delete public/index.html.
  # map.root :controller => "welcome"

  # See how all your routes lay out with "rake routes"

  # Install the default routes as the lowest priority.
  # Note: These default routes make all actions in every controller accessible via GET requests. You should
  # consider removing the them or commenting them out if you're using named routes and resources.
  map.connect ':controller/:action/:id'
  map.connect ':controller/:action/:id.:format'
end
