class Renter < ActiveRecord::Base
  #has_many :modifications
  has_many :transactions
  has_many :favorites
  has_one :user
end
