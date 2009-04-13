class Renter < ActiveRecord::Base
  #has_many :modifications
  has_many :transactions
  belongs_to :user
end
