class Profile < ActiveRecord::Base
  has_many :user_reviews
  belongs_to :college
  has_one :user
  
end
