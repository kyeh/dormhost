class Host < ActiveRecord::Base
  #has_many :modifications
  #has_many :transations
  has_many :rooms
  has_one :user
  
  acts_as_xapian  :texts => [:user_id]
  
end
