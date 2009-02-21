class CreateColleges < ActiveRecord::Migration
  def self.up
    create_table :colleges do |t|
      t.column  :name,              :string
      t.column  :street_address_1,  :string
      t.column  :street_address_2,  :string
      t.column  :city,              :string,  :limit => 50
      t.column  :state,             :string,  :limit => 2
      t.column  :zip,               :string,  :limit => 12
      t.column  :policy,            :string

      t.timestamps
    end
  end

  def self.down
    drop_table :colleges
  end
end
